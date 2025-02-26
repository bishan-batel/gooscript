//
// Created by bishan_ on 5/15/24.
//

#include "Intepreter.hpp"

#include <utility>

#include "PrimitiveOperations.hpp"
#include "ast/expression/Binary.hpp"
#include "ast/expression/FunctionCall.hpp"
#include "ast/expression/IdentifierBinding.hpp"
#include "ast/expression/compound/If.hpp"
#include "ast/expression/compound/Match.hpp"
#include "ast/expression/compound/ScopeBlock.hpp"
#include "ast/expression/compound/While.hpp"
#include "ast/expression/literal/Boolean.hpp"
#include "ast/expression/literal/Decimal.hpp"
#include "ast/expression/literal/Integer.hpp"
#include "ast/expression/literal/Lambda.hpp"
#include "ast/expression/literal/StringLiteral.hpp"
#include "ast/statements/Eval.hpp"
#include "ast/statements/Return.hpp"
#include "ast/statements/VariableDeclaration.hpp"
#include "data/Boolean.hpp"
#include "data/Decimal.hpp"
#include "data/interfaces/ICallable.hpp"
#include "data/Integer.hpp"
#include "data/Nil.hpp"
#include "data/GString.hpp"
#include "data/Unit.hpp"
#include "data/interfaces/IValue.hpp"
#include "err/NotCallable.hpp"
#include "Environment.hpp"
#include "ast/expression/ArrayIndex.hpp"
#include "ast/expression/PropertyAccess.hpp"
#include "ast/expression/Unary.hpp"
#include "ast/expression/literal/Dictionary.hpp"
#include "builtin/StandardEnviornment.hpp"
#include "data/Closure.hpp"
#include "data/Dictionary.hpp"
#include "data/Array.hpp"

namespace goos::runtime {
  Intepreter::Intepreter(RcMut<Environment> globals) : current_environment{std::move(globals)} {}

  Intepreter::Intepreter() : Intepreter{builtin::get_standard_enviornment()} {}

  // Scope Functions
  auto Intepreter::set_env(RcMut<Environment> environment) -> void {
    current_environment = std::move(environment);
  }

  auto Intepreter::get_env() -> RcMut<Environment> {
    return current_environment;
  }

  auto Intepreter::push_env(RcMut<Environment> environment) -> Environment& {
    set_env(Environment::enclose(std::move(environment)));
    return current_environment;
  }

  auto Intepreter::push_env() -> Environment& {
    return push_env(std::move(current_environment));
  }

  auto Intepreter::pop_env() -> Environment& {
    auto prev = current_environment->get_previous();
    debug_assert(prev.is_some(), "Incorrect pairing of 'pop' vs 'push'");
    current_environment = prev.take_unchecked();
    return current_environment;
  }

  auto Intepreter::env() const -> Environment& {
    return current_environment;
  }

  // Halting / Breaking Functions
  auto Intepreter::halt(const ControlFlowFlag flag, Any value) -> void {
    halt_flags.set(static_cast<u32>(flag));
    debug_assert(halt_evaluation.is_none(), "Invaliod Halting");
    halt_evaluation = crab::some(std::move(value));
  }

  auto Intepreter::should_halt_control_flow() const -> bool {
    return halt_flags.any();
  }

  auto Intepreter::consume_halt_flag(const ControlFlowFlag flag) -> Option<Any> {
    if (halt_flags[static_cast<usize>(flag)]) {
      halt_flags.reset(static_cast<usize>(flag));
      debug_assert(halt_evaluation.is_some(), "Invalid Halting");
      return std::exchange(halt_evaluation, crab::none);
    }
    return crab::none;
  }

  auto Intepreter::execute(const ast::Statement &statement) -> Result<unit, Box<err::Error>> {
    return statement.accept(*this).map_err(
      [](Box<crab::Error> err) {
        crab::Error *unwrapped = Box<crab::Error>::unwrap(std::move(err));

        return Box<err::Error>::wrap_unchecked(dynamic_cast<err::Error*>(unwrapped));
      }
    );
  }

  auto Intepreter::evaluate(const ast::Expression &expr) -> Result<Any, Box<err::Error>> {
    auto res = evaluate_shallow(expr);
    if (res.is_err()) return res.take_err_unchecked();

    Any value = res.take_unchecked();

    if (auto lvalue = value.downcast<LValue>(); lvalue.is_some()) {
      // expand out lvalue
      return Any{lvalue.take_unchecked()->get()};
    }

    return value;
  }

  auto Intepreter::evaluate_shallow(const ast::Expression &expr) -> Result<Any, Box<err::Error>> {
    return expr.accept_expr(*this).map_err(
      [](Box<crab::Error> err) {
        return Box<err::Error>::wrap_unchecked(
          dynamic_cast<Box<err::Error>::MutPtr>(Box<crab::Error>::unwrap(std::move(err)))
        );
      }
    ).map(
      [](std::any val) {
        return std::any_cast<Any>(std::move(val));
      }
    );
  }

  // AST Expansion Methods

  auto Intepreter::visit_eval(
    [[maybe_unused]
    ] const ast::Eval &eval
  ) -> Result<unit, Box<crab::Error>> {
    auto expr = evaluate(eval.get_expression());
    if (expr.is_err()) return Box<crab::Error>{expr.take_err_unchecked()};

    if (should_halt_control_flow()) {
      return unit::val;
    }

    halt(ControlFlowFlag::EVAL, expr.take_unchecked());

    return unit::val;
  }

  auto Intepreter::visit_return([[maybe_unused]] const ast::Return &ret) -> Result<unit, Box<crab::Error>> {
    auto expr = evaluate(ret.get_expression());
    if (expr.is_err()) return Box<crab::Error>(expr.take_err_unchecked());

    if (should_halt_control_flow()) {
      return unit::val;
    }

    halt(ControlFlowFlag::RETURN, expr.take_unchecked());

    return unit::val;
  }

  auto Intepreter::visit_variable_declaration(
    [[maybe_unused]] const ast::VariableDeclaration &variable_declaration
  ) -> Result<unit, Box<crab::Error>> {
    auto initial = evaluate(variable_declaration.get_initializer());

    if (initial.is_err()) return Box<crab::Error>{initial.take_err_unchecked()};

    if (should_halt_control_flow()) {
      return unit::val;
    }

    current_environment->push_variable(
      variable_declaration.get_name(),
      variable_declaration.get_mutability(),
      initial.take_unchecked()
    );
    return unit::val;
  }

  auto Intepreter::visit_array(
    [[maybe_unused]] const ast::expression::Array &array
  ) -> Result<std::any, Box<crab::Error>> {
    const RcMut<Array> arr = crab::make_rc_mut<Array>();

    for (const auto &val: array.get_values()) {
      Result<Any, Box<err::Error>> result = evaluate(val);

      if (result.is_err()) return error(result.take_err_unchecked());

      if (should_halt_control_flow()) return Unit::ok();

      arr->push(result.take_unchecked());
    }

    return ok(arr);
  }

  auto Intepreter::visit_dictionary(
    [[maybe_unused]] const ast::expression::Dictionary &dictionary
  ) -> Result<std::any, Box<crab::Error>> {
    const auto dict = crab::make_rc_mut<Dictionary>();

    for (const auto &[key , value]: dictionary.get_pairs()) {
      Any key_expr = Unit::value();

      if (auto ident = key->try_as<ast::expression::IdentifierBinding>()) {
        key_expr = crab::make_rc_mut<GString>(ident.get_unchecked()->get_identifier());
      } else {
        auto key_result = evaluate(key);
        if (key_result.is_err()) return error(key_result.take_err_unchecked());

        if (should_halt_control_flow()) {
          // return unit
          return Unit::ok();
        }

        key_expr = key_result.take_unchecked();
      }

      auto value_result = evaluate(value);
      if (value_result.is_err()) return error(value_result.take_err_unchecked());

      if (should_halt_control_flow()) {
        // return unit
        return Unit::ok();
      }
      dict->set(std::move(key_expr), value_result.take_unchecked());
    }

    return ok(dict);
  }

  auto Intepreter::visit_lambda(
    [[maybe_unused]] const ast::expression::Lambda &lambda
  ) -> Result<std::any, Box<crab::Error>> {
    using namespace ast;
    using namespace expression;

    return ok(
      crab::make_rc_mut<Closure>(
        get_env(),
        Box<Lambda>::wrap_unchecked(dynamic_cast<Lambda*>(Box<Expression>::unwrap(lambda.clone_expr())))
      )
    );
  }

  auto Intepreter::visit_string_literal(
    [[maybe_unused]] const ast::expression::StringLiteral &str
  ) -> Result<std::any, Box<crab::Error>> {
    return ok(crab::make_rc_mut<GString>(str.get_string()));
  }

  auto Intepreter::visit_boolean(
    [[maybe_unused]] const ast::expression::Boolean &boolean
  ) -> Result<std::any, Box<crab::Error>> {
    return ok(crab::make_rc_mut<Boolean>(boolean.get_state()));
  }

  auto Intepreter::visit_decimal(
    [[maybe_unused]] const ast::expression::Decimal &decimal
  ) -> Result<std::any, Box<crab::Error>> {
    return ok(crab::make_rc_mut<Decimal>(decimal.get_number()));
  }

  auto Intepreter::visit_integer(
    [[maybe_unused]] const ast::expression::Integer &integer
  ) -> Result<std::any, Box<crab::Error>> {
    return ok(crab::make_rc_mut<Integer>(integer.get_number()));
  }

  auto Intepreter::visit_nil([[maybe_unused]] const ast::expression::Nil &nil) -> Result<std::any, Box<crab::Error>> {
    return Nil::ok();
  }

  auto Intepreter::visit_unit(
    [[maybe_unused]] const ast::expression::Unit &unit
  ) -> Result<std::any, Box<crab::Error>> {
    return ok(crab::make_rc_mut<Unit>());
  }

  auto Intepreter::visit_binary(
    [[maybe_unused]] const ast::expression::Binary &binary
  ) -> Result<std::any, Box<crab::Error>> {
    if (binary.get_op() == lexer::Operator::ASSIGN) {
      Result<Any, Box<err::Error>> lhs_result = evaluate_shallow(binary.get_lhs());
      if (lhs_result.is_err()) return error(lhs_result.take_err_unchecked());

      if (should_halt_control_flow()) {
        return Unit::ok();
      }

      const RcMut<LValue> lvalue = lhs_result.take_unchecked().downcast<LValue>().take_unchecked();

      Result<Any, Box<err::Error>> value_result = evaluate(binary.get_rhs());
      if (value_result.is_err()) return error(value_result.take_err_unchecked());

      const Any value = value_result.take_unchecked();

      if (should_halt_control_flow()) {
        return Unit::ok();
      }

      lvalue->set(value);

      return ok(lvalue);
    }

    if (binary.get_op() == lexer::Operator::DOT) {
      const meta::Identifier rhs_identifier =
          binary
          .get_rhs()
          .try_as<ast::expression::IdentifierBinding>()
          .take_unchecked()
          ->get_identifier();

      Result<Any, Box<err::Error>> dictionary_value = evaluate(binary.get_lhs());

      if (dictionary_value.is_err()) {
        return error(dictionary_value.take_err_unchecked());
      }

      // TODO: error handling

      if (should_halt_control_flow()) {
        return Unit::ok();
      }

      const RcMut<Dictionary> dict = dictionary_value.take_unchecked().downcast<Dictionary>().take_unchecked();

      if (auto value = dict->get(rhs_identifier)) {
        return ok(value.take_unchecked());
      }

      return Nil::ok();
    }

    // LHS
    Result<Any, Box<err::Error>> lhs_result{evaluate(binary.get_lhs())};
    if (lhs_result.is_err()) return error(lhs_result.take_err_unchecked());

    if (should_halt_control_flow()) return Unit::ok();

    const Any lhs_val{lhs_result.take_unchecked()};

    // RHS
    Result<Any, Box<err::Error>> rhs_result{evaluate(binary.get_rhs())};

    if (rhs_result.is_err()) return error(rhs_result.take_err_unchecked());

    if (should_halt_control_flow()) return Unit::ok();
    const Any rhs_val{rhs_result.take_unchecked()};

    // TODO all assign cases

    using namespace primitive_operators;

    if (const BinaryOperands index{lhs_val->get_type(), rhs_val->get_type(), binary.get_op()}; BINARY_FUNCTIONS.
      contains(
        index
      )) {
      const BinaryFunction func = BINARY_FUNCTIONS.at(index);
      return ok(func(lhs_val, rhs_val));
    }

    const auto lhs_hash = lhs_val->hash();
    const auto rhs_hash = rhs_val->hash();

    switch (binary.get_op()) {
      case lexer::Operator::EQUALS: { return ok(crab::make_rc_mut<Boolean>(lhs_hash == rhs_hash)); }
      case lexer::Operator::NOT_EQUALS: { return ok(crab::make_rc_mut<Boolean>(lhs_hash != rhs_hash)); }
      default: { return Nil::ok(); }
    }
  }

  auto Intepreter::visit_function_call(
    [[maybe_unused]] const ast::expression::FunctionCall &function_call
  ) -> Result<std::any, Box<crab::Error>> {
    auto function_result = evaluate(function_call.get_function());

    if (function_result.is_err()) {
      return error(function_result.take_err_unchecked());
    }

    if (should_halt_control_flow()) {
      return ok(function_result.take_unchecked());
    }

    const auto callable_any = function_result.take_unchecked();

    auto callable_option = callable_any.downcast<ICallable>();

    // TODO
    if (callable_option.is_none()) {
      return error(crab::make_box<err::NotCallable>(callable_any));
    }

    const RcMut<ICallable> callable = callable_option.take_unchecked();

    Vec<Any> arguments;
    arguments.reserve(function_call.get_arguments().size());

    for (auto &arg: function_call.get_arguments()) {
      auto value_result = evaluate(arg);

      if (value_result.is_err()) {
        return error(value_result.take_err_unchecked());
      }

      auto value = value_result.take_unchecked();

      if (should_halt_control_flow()) {
        return ok(value);
      }

      arguments.push_back(std::move(value));
    }

    push_env();
    auto result = callable->call(*this, arguments);
    pop_env();

    if (result.is_err()) return error(result.take_err_unchecked());
    return ok(result.take_unchecked());
  }

  auto Intepreter::visit_identifier_binding(
    [[maybe_unused]] const ast::expression::IdentifierBinding &identifier
  ) -> Result<std::any, Box<crab::Error>> {
    Result<RcMut<Variable>, Box<err::Error>> var = current_environment->get_variable(identifier.get_identifier());
    if (var.is_err()) {
      return error(var.take_err_unchecked());
    }

    return ok(var.take_unchecked()->as_lvalue());
  }

  auto Intepreter::visit_unary(
    [[maybe_unused]] const ast::expression::Unary &unary
  ) -> Result<std::any, Box<crab::Error>> {
    auto value_result{evaluate(unary.get_expression())};

    if (value_result.is_err()) return error(value_result.take_err_unchecked());

    if (should_halt_control_flow()) {
      return Unit::ok();
    }

    auto value = value_result.take_unchecked();

    using namespace primitive_operators;
    if (const UnaryOperands index{value->get_type(), unary.get_op()};
      UNARY_FUNCTIONS.contains(index)
    ) {
      return ok(UNARY_FUNCTIONS.at(index)(std::move(value)));
    }

    return Unit::ok();
  }

  auto Intepreter::visit_if([[maybe_unused]] const ast::expression::If &if_expr) -> Result<std::any, Box<crab::Error>> {
    Result<Any, Box<err::Error>> condition = evaluate(if_expr.get_condition());

    if (condition.is_err()) return error(condition.take_err_unchecked());

    if (should_halt_control_flow()) {
      return Unit::ok();
    }

    if (condition.take_unchecked()->is_truthy()) {
      Result<Any, Box<err::Error>> then = evaluate(if_expr.get_then());
      if (then.is_err()) return error(then.take_err_unchecked());

      if (should_halt_control_flow()) {
        return Unit::ok();
      }

      return ok(then.take_unchecked());
    }

    Result<Any, Box<err::Error>> else_then = evaluate(if_expr.get_else_then());
    if (else_then.is_err()) return error(else_then.take_err_unchecked());

    if (should_halt_control_flow()) {
      return Unit::ok();
    }

    return ok(else_then.take_unchecked());
  }

  auto Intepreter::visit_scope(
    [[maybe_unused]] const ast::expression::ScopeBlock &scope
  ) -> Result<std::any, Box<crab::Error>> {
    push_env();
    for (const auto &statement: scope.get_statements()) {
      if (auto err = execute(statement); err.is_err()) {
        return error(err.take_err_unchecked());
      }

      if (should_halt_control_flow()) {
        if (auto evaluation = consume_halt_flag(ControlFlowFlag::EVAL)) {
          return ok(evaluation.take_unchecked());
        }
        pop_env();
        return Unit::ok();
      }
    }

    Result<Any, Box<err::Error>> value_res = evaluate(scope.get_eval());

    if (value_res.is_err()) return error(value_res.take_err_unchecked());

    pop_env();

    const Any value = value_res.take_unchecked();

    if (auto evaluation = consume_halt_flag(ControlFlowFlag::EVAL)) {
      return ok(evaluation.take_unchecked());
    }

    if (should_halt_control_flow()) {
      return Unit::ok();
    }

    return ok(value);
  }

  auto Intepreter::visit_while(
    [[maybe_unused]] const ast::expression::While &while_expr
  ) -> Result<std::any, Box<crab::Error>> {
    Any eval = Unit::value();

    while (true) {
      Result<Any, Box<err::Error>> condition = evaluate(while_expr.get_condition());
      if (condition.is_err()) return error(condition.take_err_unchecked());

      if (should_halt_control_flow()) {
        return Unit::ok();
      }

      if (not condition.take_unchecked()->is_truthy()) {
        break;
      }

      Result<Any, Box<err::Error>> body = evaluate(while_expr.get_body());
      if (body.is_err()) return error(body.take_err_unchecked());

      if (auto value = consume_halt_flag(ControlFlowFlag::BREAK_LOOP)) {
        return ok(value.take_unchecked());
      }

      if (should_halt_control_flow()) {
        return Unit::ok();
      }
      eval = body.take_unchecked();
    }

    return ok(eval);
  }

  auto Intepreter::visit_property_access(
    const ast::expression::PropertyAccess &property_access
  ) -> Result<std::any, Box<crab::Error>> {
    Result<Any, Box<err::Error>> obj_result = evaluate(property_access.get_object());
    if (obj_result.is_err()) return error(obj_result.take_err_unchecked());

    if (should_halt_control_flow()) {
      return Unit::ok();
    }

    auto casted_opt = obj_result.take_unchecked().downcast<Dictionary>();

    if (casted_opt.is_none()) {
      return Unit::ok();
    }

    const meta::Identifier property = property_access.get_property();
    Dictionary &dict = casted_opt.take_unchecked();

    if (auto value = dict.get_lvalue(property); value.is_some()) {
      return ok(value.take_unchecked());
    }

    dict.set(property, Unit::value());

    return ok(dict.get_lvalue(property).take_unchecked());
  }

  auto Intepreter::visit_array_index(
    const ast::expression::ArrayIndex &array_index
  ) -> Result<std::any, Box<crab::Error>> {
    Result<Any, Box<err::Error>> obj_result = evaluate(array_index.get_object());
    if (obj_result.is_err()) return error(obj_result.take_err_unchecked());

    if (should_halt_control_flow()) return Unit::ok();

    Result<Any, Box<err::Error>> index_result = evaluate(array_index.get_index());
    if (index_result.is_err()) return error(index_result.take_err_unchecked());

    if (should_halt_control_flow()) return Unit::ok();

    const Any obj = obj_result.take_unchecked();
    Any index = index_result.take_unchecked();

    if (auto opt = obj.downcast<IIndexible>()) {
      return opt.take_unchecked()->index(std::move(index))
                .map([](const Any &v) { return std::any{v}; })
                .map_err(
                  [](auto err) {
                    return Box<crab::Error>{std::move(err)};
                  }
                );
    }

    // TODO, make an error
    return Nil::ok();
  }

  auto Intepreter::visit_match(const ast::expression::Match &) -> Result<std::any, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }
}
