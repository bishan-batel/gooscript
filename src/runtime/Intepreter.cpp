//
// Created by bishan_ on 5/15/24.
//

#include "Intepreter.hpp"

#include <memory>

#include "ast/expression/Binary.hpp"
#include "ast/expression/FunctionCall.hpp"
#include "ast/expression/IdentifierBinding.hpp"
#include "ast/expression/compound/ScopeBlock.hpp"
#include "ast/expression/literal/Boolean.hpp"
#include "ast/expression/literal/Decimal.hpp"
#include "ast/expression/literal/Integer.hpp"
#include "ast/expression/literal/StringLiteral.hpp"
#include "ast/statements/VariableDeclaration.hpp"
#include "data/Boolean.hpp"
#include "data/Decimal.hpp"
#include "data/Integer.hpp"
#include "data/Nil.hpp"
#include "data/StringLiteral.hpp"
#include "data/Unit.hpp"
#include "data/Value.hpp"
#include "utils/str.hpp"

namespace goos::runtime {
  auto Intepreter::push_env() -> Environment& {
    env = crab::make_rc_mut<Environment>(env);
    return env;
  }

  auto Intepreter::pop_env() -> Environment& {
    auto prev = env->get_previous();
    debug_assert(prev.is_some(), "Incorrect pairing of 'pop' vs 'push'");
    env = prev.take_unchecked();
    return env;
  }

  auto Intepreter::execute(const ast::Statement &statement) -> VoidResult {
    return statement.accept(*this);
  }

  auto Intepreter::evaluate(const ast::Expression &expr) -> Result<Any> {
    return expr.accept_expr(*this);
  }

  auto Intepreter::visit_eval([[maybe_unused]] const ast::Eval &eval) -> VoidResult {
    throw std::logic_error("Not implemented: eval");
    return crab::none;
  }

  auto Intepreter::visit_return([[maybe_unused]] const ast::Return &ret) -> VoidResult {
    throw std::logic_error("Not implemented: return");
    return crab::none;
  }

  auto Intepreter::visit_variable_declaration(
    [[maybe_unused]] const ast::VariableDeclaration &variable_declaration
  ) -> VoidResult {
    auto initial = evaluate(variable_declaration.get_initializer());

    if (initial.is_err()) return some(initial.take_err_unchecked());

    env->push_variable(
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
    throw std::logic_error("Not implemented");
  }

  auto Intepreter::visit_lambda([[maybe_unused]] const ast::expression::Lambda &lambda) -> Result<Any> {
    throw std::logic_error("Not implemented");
  }

  auto Intepreter::visit_string_literal([[maybe_unused]] const ast::expression::StringLiteral &str) -> Result<Any> {
    return ok(crab::make_rc_mut<StringLiteral>(str.get_string()));
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
    return ok(crab::make_rc_mut<Nil>());
  }

  auto Intepreter::visit_unit([[maybe_unused]] const ast::expression::Unit &unit) -> Result<Any> {
    return ok(crab::make_rc_mut<Unit>());
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

      env->set_value(lhs_val->get_identifier(), value);
      return ok(value);
    }

    auto lhs_result{evaluate(binary.get_lhs())};
    auto rhs_result{evaluate(binary.get_rhs())};

    if (lhs_result.is_err()) return crab::err(lhs_result.take_err_unchecked());
    if (rhs_result.is_err()) return crab::err(rhs_result.take_err_unchecked());

    const auto lhs_val = lhs_result.take_unchecked();
    const auto rhs_val = rhs_result.take_unchecked();
    // TODO all assign cases

    if (lhs_val->get_type() != rhs_val->get_type()) return ok(crab::make_rc_mut<Nil>());

    switch (lhs_val->get_type()) {
      case meta::VariantType::INTEGER: {
        const auto lhs = lhs_val->coerce_unchecked<Integer>().get();
        const auto rhs = rhs_val->coerce_unchecked<Integer>().get();

        switch (binary.get_op()) {
          case lexer::Operator::XOR:
            return ok(crab::make_rc_mut<Integer>(lhs xor rhs));
          case lexer::Operator::BIT_OR:
            return ok(crab::make_rc_mut<Integer>(lhs bitor rhs));
          case lexer::Operator::BIT_AND:
            return ok(crab::make_rc_mut<Integer>(lhs bitand rhs));
          case lexer::Operator::EQUALS:
            return ok(crab::make_rc_mut<Integer>(lhs == rhs));
          case lexer::Operator::NOT_EQUALS:
            return ok(crab::make_rc_mut<Integer>(lhs != rhs));
          case lexer::Operator::GREATER:
            return ok(crab::make_rc_mut<Integer>(lhs > rhs));
          case lexer::Operator::LESS:
            return ok(crab::make_rc_mut<Integer>(lhs < rhs));
          case lexer::Operator::GREATER_OR_EQUALS:
            return ok(crab::make_rc_mut<Integer>(lhs >= rhs));
          case lexer::Operator::LESS_OR_EQUALS:
            return ok(crab::make_rc_mut<Integer>(lhs <= rhs));
          case lexer::Operator::SHIFT_LEFT:
            return ok(crab::make_rc_mut<Integer>(lhs << rhs));
          case lexer::Operator::SHIFT_RIGHT:
            return ok(crab::make_rc_mut<Integer>(lhs >> rhs));
          case lexer::Operator::ADD:
            return ok(crab::make_rc_mut<Integer>(lhs + rhs));
          case lexer::Operator::SUB:
            return ok(crab::make_rc_mut<Integer>(lhs - rhs));
          case lexer::Operator::MUL:
            return ok(crab::make_rc_mut<Integer>(lhs * rhs));
          case lexer::Operator::DIV:
            return ok(crab::make_rc_mut<Integer>(lhs / rhs));
          case lexer::Operator::MOD:
            return ok(crab::make_rc_mut<Integer>(lhs % rhs));
          default: break;
        }
      }

      default: break;
    }

    return ok(crab::make_rc_mut<Nil>());
  }

  auto Intepreter::visit_function_call(
    [[maybe_unused]] const ast::expression::FunctionCall &function_call
  ) -> Result<Any> {
    std::wcout << "funcall";
    // std::wcout << execute(function_call.get_function())->to_string();
    // std::wcout << '(';
    // std::wcout << str::join(
    //   function_call.get_arguments(),
    //   [this](auto &arg) {
    //     return evaluate(arg).get_unchecked()->to_string();
    //   }
    // );
    std::wcout << ')' << std::endl;

    return ok(crab::make_rc_mut<Unit>());
  }

  auto Intepreter::visit_identifier_binding(
    [[maybe_unused]] const ast::expression::IdentifierBinding &identifier
  ) -> Result<Any> {
    return ok(env->get_variable(identifier.get_identifier())->get_value());
  }

  auto Intepreter::visit_unary([[maybe_unused]] const ast::expression::Unary &unary) -> Result<Any> {
    throw std::logic_error("Not implemented: unary");
  }

  auto Intepreter::visit_if([[maybe_unused]] const ast::expression::If &if_expr) -> Result<Any> {
    throw std::logic_error("Not implemented: if");
  }

  auto Intepreter::visit_scope([[maybe_unused]] const ast::expression::ScopeBlock &scope) -> Result<Any> {
    for (const auto &statement: scope.get_statements()) {
      execute(statement);
    }
    return evaluate(scope.get_eval());
  }

  auto Intepreter::visit_while([[maybe_unused]] const ast::expression::While &while_expr) -> Result<Any> {
    throw std::logic_error("Not implemented: while");
  }
}
