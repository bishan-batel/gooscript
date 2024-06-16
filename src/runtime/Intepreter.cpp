//
// Created by bishan_ on 5/15/24.
//

#include "Intepreter.hpp"

#include <memory>

#include "PrimitiveOperations.hpp"
#include "ast/expression/Binary.hpp"
#include "ast/expression/FunctionCall.hpp"
#include "ast/expression/IdentifierBinding.hpp"
#include "ast/expression/compound/If.hpp"
#include "ast/expression/compound/ScopeBlock.hpp"
#include "ast/expression/compound/While.hpp"
#include "ast/expression/literal/Boolean.hpp"
#include "ast/expression/literal/Decimal.hpp"
#include "ast/expression/literal/Integer.hpp"
#include "ast/expression/literal/Lambda.hpp"
#include "ast/expression/literal/StringLiteral.hpp"
#include "ast/statements/Return.hpp"
#include "ast/statements/VariableDeclaration.hpp"
#include "data/Boolean.hpp"
#include "data/ExternFunction.hpp"
#include "data/Decimal.hpp"
#include "data/ICallable.hpp"
#include "data/Integer.hpp"
#include "data/Nil.hpp"
#include "data/GString.hpp"
#include "data/Unit.hpp"
#include "data/IValue.hpp"
#include "err/NotCallable.hpp"
#include "Environment.hpp"
#include "ast/expression/literal/Dictionary.hpp"
#include "data/Dictionary.hpp"
#include "utils/str.hpp"

namespace goos::runtime {
  auto Intepreter::push_env() -> Environment& {
    current_environment = Environment::enclose(current_environment);
    return current_environment;
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

  Intepreter::Intepreter() : current_environment{Environment::get_standard_environment(*this)} {}

  auto Intepreter::execute(const ast::Statement &statement) -> VoidResult {
    return statement.accept(*this);
  }

  auto Intepreter::evaluate(const ast::Expression &expr) -> Result<Any> {
    return expr.accept_expr(*this);
  }

  auto Intepreter::visit_eval([[maybe_unused]] const ast::Eval &eval) -> VoidResult {
    auto expr = evaluate(eval.get_expression());
    if (expr.is_err()) return some(expr.take_err_unchecked());

    halt(ControlFlowFlag::EVAL, expr.take_unchecked());

    return crab::none;
  }

  auto Intepreter::visit_return([[maybe_unused]] const ast::Return &ret) -> VoidResult {
    auto expr = evaluate(ret.get_expression());
    if (expr.is_err()) return some(expr.take_err_unchecked());

    halt(ControlFlowFlag::RETURN, expr.take_unchecked());

    return crab::none;
  }

  auto Intepreter::visit_variable_declaration(
    [[maybe_unused]] const ast::VariableDeclaration &variable_declaration
  ) -> VoidResult {
    auto initial = evaluate(variable_declaration.get_initializer());

    if (initial.is_err()) return some(initial.take_err_unchecked());

    current_environment->push_variable(
      variable_declaration.get_name(),
      variable_declaration.get_mutability(),
      initial.take_unchecked()
    );
    return crab::none;
  }

  auto Intepreter::visit_array([[maybe_unused]] const ast::expression::Array &array) -> Result<Any> {
    throw std::logic_error("Not implemented");
  }

  auto Intepreter::visit_dictionary([[maybe_unused]] const ast::expression::Dictionary &dictionary) -> Result<Any> {
    const auto dict = crab::make_rc_mut<Dictionary>();
    // throw std::logic_error("Not implemented");

    for (const auto &[key , value]: dictionary.get_pairs()) {
      Any key_expr = Unit::value();

      if (auto ident = key->try_as<ast::expression::IdentifierBinding>()) {
        key_expr = crab::make_rc_mut<GString>(ident.get_unchecked()->get_identifier());
      } else {
        auto key_result = evaluate(key);
        if (key_result.is_err()) return crab::err(key_result.take_err_unchecked());

        if (should_halt_control_flow()) {
          // return unit
          return ok(Unit::value());
        }

        key_expr = key_result.take_unchecked();
      }

      auto value_result = evaluate(value);
      if (value_result.is_err()) return crab::err(value_result.take_err_unchecked());

      if (should_halt_control_flow()) {
        // return unit
        return ok(Unit::value());
      }
      dict->set(std::move(key_expr), value_result.take_unchecked());
    }

    return ok(dict);
  }

  auto Intepreter::visit_lambda([[maybe_unused]] const ast::expression::Lambda &lambda) -> Result<Any> {
    // TODO

    const auto closure = Rc<ast::Expression>::from_owned_unchecked(
      Box<ast::Expression>::unwrap(lambda.get_body().clone_expr())
    );

    return ok(
      ExternFunction::varargs(
        [closure, &lambda](Environment &env, const Vec<Any> &args) -> Result<Any> {
          const auto &params = lambda.get_params();
          const usize count = std::min(args.size(), params.size());

          for (usize i = 0; i < count; i++) {
            env.push_variable(
              meta::Identifier{params[i]},
              meta::Mutability::IMMUTABLE,
              args[i]
            );
          }

          auto value = env.runtime().evaluate(closure);

          if (value.is_err()) {
            return crab::err(value.take_err_unchecked());
          }

          if (auto ret = env.runtime().consume_halt_flag(ControlFlowFlag::RETURN)) {
            return crab::ok(ret.take_unchecked());
          }

          return crab::ok(value.take_unchecked());
        }
      )
    );
  }

  auto Intepreter::visit_string_literal([[maybe_unused]] const ast::expression::StringLiteral &str) -> Result<Any> {
    return ok(crab::make_rc_mut<GString>(str.get_string()));
  }

  auto Intepreter::visit_boolean([[maybe_unused]] const ast::expression::Boolean &boolean) -> Result<Any> {
    return ok(crab::make_rc_mut<Boolean>(boolean.get_state()));
  }

  auto Intepreter::visit_decimal([[maybe_unused]] const ast::expression::Decimal &decimal) -> Result<Any> {
    return ok(crab::make_rc_mut<Decimal>(decimal.get_number()));
  }

  auto Intepreter::visit_integer([[maybe_unused]] const ast::expression::Integer &integer) -> Result<Any> {
    return ok(crab::make_rc_mut<Integer>(integer.get_number()));
  }

  auto Intepreter::visit_nil([[maybe_unused]] const ast::expression::Nil &nil) -> Result<Any> {
    return ok(Nil::value());
  }

  auto Intepreter::visit_unit([[maybe_unused]] const ast::expression::Unit &unit) -> Result<Any> {
    return ok(Unit::value());
  }

  auto Intepreter::visit_binary([[maybe_unused]] const ast::expression::Binary &binary) -> Result<Any> {
    if (binary.get_op() == lexer::Operator::ASSIGN) {
      const auto lhs_val = binary
                           .get_lhs()
                           .try_as<ast::expression::IdentifierBinding>()
                           .take_unchecked();

      Result<Any> value_result = evaluate(binary.get_rhs());

      if (value_result.is_err()) {
        return crab::err(value_result.take_err_unchecked());
      }

      const Any value = value_result.take_unchecked();

      if (should_halt_control_flow()) {
        return ok(value);
      }

      if (auto err = current_environment->set_value(lhs_val->get_identifier(), value); err.is_err())
        return crab::err(err.take_err_unchecked());

      return ok(value);
    }

    if (binary.get_op() == lexer::Operator::DOT) {
      const meta::Identifier rhs_identifier =
          binary
          .get_rhs()
          .try_as<ast::expression::IdentifierBinding>()
          .take_unchecked()
          ->get_identifier();

      Result<Any> dictionary_value = evaluate(binary.get_lhs());

      if (dictionary_value.is_err()) {
        return crab::err(dictionary_value.take_err_unchecked());
      }

      // TODO: error handling

      if (should_halt_control_flow()) {
        return ok(Unit::value());
      }

      const RcMut<Dictionary> dict = dictionary_value.take_unchecked().downcast<Dictionary>().take_unchecked();

      if (auto value = dict->get(rhs_identifier)) {
        return ok(value.take_unchecked());
      }

      return ok(Nil::value());
    }

    auto lhs_result{evaluate(binary.get_lhs())};
    auto rhs_result{evaluate(binary.get_rhs())};

    if (lhs_result.is_err()) return crab::err(lhs_result.take_err_unchecked());
    if (rhs_result.is_err()) return crab::err(rhs_result.take_err_unchecked());

    const auto lhs_val = lhs_result.take_unchecked();
    const auto rhs_val = rhs_result.take_unchecked();
    // TODO all assign cases

    using namespace primitive_operators;

    if (const Operands index{lhs_val->get_type(), rhs_val->get_type(), binary.get_op()}; BINARY_FUNCTIONS.
      contains(
        index
      )) {
      const BinaryFunction func = BINARY_FUNCTIONS.at(index);
      return ok(func(lhs_val, rhs_val));
    }

    const auto lhs_hash = lhs_val->hash();
    const auto rhs_hash = lhs_val->hash();

    switch (binary.get_op()) {
      case lexer::Operator::EQUALS: { return ok(crab::make_rc_mut<Boolean>(lhs_hash == rhs_hash)); }
      case lexer::Operator::NOT_EQUALS: { return ok(crab::make_rc_mut<Boolean>(lhs_hash != rhs_hash)); }
      default: { return ok(Nil::value()); }
    }
  }

  auto Intepreter::visit_function_call(
    [[maybe_unused]] const ast::expression::FunctionCall &function_call
  ) -> Result<Any> {
    auto function_result = evaluate(function_call.get_function());

    if (function_result.is_err()) {
      return crab::err(function_result.take_err_unchecked());
    }

    if (should_halt_control_flow()) {
      return function_result;
    }

    auto callable_any = function_result.take_unchecked();

    auto callable_option = callable_any.downcast<ICallable>();

    if (callable_option.is_none()) {
      return crab::err<Box<err::Error>>(
        crab::make_box<err::NotCallable>(std::move(callable_any))
      );
    }

    const RcMut<ICallable> callable = callable_option.take_unchecked();

    Vec<Any> arguments;
    arguments.reserve(function_call.get_arguments().size());

    for (auto &arg: function_call.get_arguments()) {
      auto value_result = evaluate(arg);

      if (value_result.is_err()) {
        return crab::err(value_result.take_err_unchecked());
      }

      auto value = value_result.take_unchecked();

      if (should_halt_control_flow()) {
        return ok(value);
      }

      arguments.push_back(std::move(value));
    }

    push_env();
    auto result = callable->call(current_environment, arguments);
    pop_env();
    return result;
  }

  auto Intepreter::visit_identifier_binding(
    [[maybe_unused]] const ast::expression::IdentifierBinding &identifier
  ) -> Result<Any> {
    Result<RcMut<Variable>> var = current_environment->get_variable(identifier.get_identifier());
    if (var.is_err()) {
      return crab::err(var.take_err_unchecked());
    }

    return crab::ok(var.take_unchecked()->get_value());
  }

  auto Intepreter::visit_unary([[maybe_unused]] const ast::expression::Unary &unary) -> Result<Any> {
    throw std::logic_error("Not implemented: unary");
  }

  auto Intepreter::visit_if([[maybe_unused]] const ast::expression::If &if_expr) -> Result<Any> {
    Result<Any> condition = evaluate(if_expr.get_condition());

    if (condition.is_err()) return crab::err(condition.take_err_unchecked());

    if (should_halt_control_flow()) {
      return ok(Unit::value());
    }

    if (condition.take_unchecked()->is_truthy()) {
      Result<Any> then = evaluate(if_expr.get_then());
      if (then.is_err()) return crab::err(then.take_err_unchecked());

      if (should_halt_control_flow()) {
        return ok(Unit::value());
      }

      return ok(then.take_unchecked());
    }

    Result<Any> else_then = evaluate(if_expr.get_else_then());
    if (else_then.is_err()) return crab::err(else_then.take_err_unchecked());

    if (should_halt_control_flow()) {
      return ok(Unit::value());
    }

    return ok(else_then.take_unchecked());
  }

  auto Intepreter::visit_scope([[maybe_unused]] const ast::expression::ScopeBlock &scope) -> Result<Any> {
    push_env();
    for (const auto &statement: scope.get_statements()) {
      if (auto err = execute(statement)) {
        return crab::err(err.take_unchecked());
      }

      if (should_halt_control_flow()) {
        if (auto evaluation = consume_halt_flag(ControlFlowFlag::EVAL)) {
          return ok(evaluation.take_unchecked());
        }
        pop_env();
        return ok(Unit::value());
      }
    }

    Result<Any> value = evaluate(scope.get_eval());

    if (value.is_err()) return crab::err(value.take_err_unchecked());

    pop_env();

    if (should_halt_control_flow()) {
      return ok(Unit::value());
    }

    return value;
  }

  auto Intepreter::visit_while([[maybe_unused]] const ast::expression::While &while_expr) -> Result<Any> {
    Any eval = Unit::value();

    while (true) {
      Result<Any> condition = evaluate(while_expr.get_condition());
      if (condition.is_err()) return crab::err(condition.take_err_unchecked());

      if (not condition.take_unchecked()->is_truthy()) {
        break;
      }

      Result<Any> body = evaluate(while_expr.get_body());
      if (body.is_err()) return crab::err(body.take_err_unchecked());

      if (auto value = consume_halt_flag(ControlFlowFlag::BREAK_LOOP)) {
        return ok(value.take_unchecked());
      }

      if (should_halt_control_flow()) {
        return ok(Unit::value());
      }
      eval = body.take_unchecked();
    }

    return ok(eval);
  }
}
