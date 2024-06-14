//
// Created by bishan_ on 5/15/24.
//

#include "Intepreter.hpp"

#include <memory>
#include <memory>
#include <memory>
#include <memory>
#include <memory>
#include <memory>
#include <memory>

#include "ast/expression/Binary.hpp"
#include "ast/expression/FunctionCall.hpp"
#include "ast/expression/compound/ScopeBlock.hpp"
#include "ast/expression/literal/Boolean.hpp"
#include "ast/expression/literal/Decimal.hpp"
#include "ast/expression/literal/Integer.hpp"
#include "data/Boolean.hpp"
#include "data/Decimal.hpp"
#include "data/Integer.hpp"
#include "data/Nil.hpp"
#include "data/Unit.hpp"
#include "data/Value.hpp"
#include "utils/str.hpp"

namespace goos::runtime {
  auto Intepreter::execute(const ast::Statement &statement) -> void {
    statement.accept(*this);
  }

  auto Intepreter::execute(const ast::Expression &expr) -> RcMut<Value> {
    return expr.accept_expr(*this);
  }

  auto Intepreter::visit_eval([[maybe_unused]] const ast::Eval &eval) -> void {
    throw std::logic_error("Not implemented: eval");
  }

  auto Intepreter::visit_return([[maybe_unused]] const ast::Return &ret) -> void {
    throw std::logic_error("Not implemented: return");
  }

  auto Intepreter::visit_variable_declaration(
    [[maybe_unused]] const ast::VariableDeclaration &variable_declaration
  ) -> void {
    throw std::logic_error("Not implemented");
  }

  auto Intepreter::visit_array([[maybe_unused]] const ast::expression::Array &array) -> RcMut<Value> {
    throw std::logic_error("Not implemented");
  }

  auto Intepreter::visit_dictionary([[maybe_unused]] const ast::expression::Dictionary &dictionary) -> RcMut<Value> {
    throw std::logic_error("Not implemented");
  }

  auto Intepreter::visit_lambda([[maybe_unused]] const ast::expression::Lambda &lambda) -> RcMut<Value> {
    throw std::logic_error("Not implemented");
  }

  auto Intepreter::visit_string_literal([[maybe_unused]] const ast::expression::StringLiteral &str) -> RcMut<Value> {
    throw std::logic_error("Not implemented");
  }

  auto Intepreter::visit_boolean([[maybe_unused]] const ast::expression::Boolean &boolean) -> RcMut<Value> {
    return crab::make_rc_mut<Boolean>(boolean.get_state());
  }

  auto Intepreter::visit_decimal([[maybe_unused]] const ast::expression::Decimal &decimal) -> RcMut<Value> {
    return crab::make_rc_mut<Decimal>(decimal.get_number());
  }

  auto Intepreter::visit_integer([[maybe_unused]] const ast::expression::Integer &integer) -> RcMut<Value> {
    return crab::make_rc_mut<Integer>(integer.get_number());
  }

  auto Intepreter::visit_nil([[maybe_unused]] const ast::expression::Nil &nil) -> RcMut<Value> {
    return crab::make_rc_mut<Nil>();
  }

  auto Intepreter::visit_unit([[maybe_unused]] const ast::expression::Unit &unit) -> RcMut<Value> {
    return crab::make_rc_mut<Unit>();
  }

  auto Intepreter::visit_binary([[maybe_unused]] const ast::expression::Binary &binary) -> RcMut<Value> {
    const auto lhs_val{execute(binary.get_lhs())};
    const auto rhs_val{execute(binary.get_rhs())};

    if (lhs_val->get_type() != rhs_val->get_type()) return crab::make_rc_mut<Nil>();

    switch (lhs_val->get_type()) {
      case meta::VariantType::INTEGER: {
        const auto lhs = lhs_val->coerce_unchecked<Integer>().get();
        const auto rhs = rhs_val->coerce_unchecked<Integer>().get();

        switch (binary.get_op()) {
          case lexer::Operator::XOR:
            return crab::make_rc_mut<Integer>(lhs xor rhs);
          case lexer::Operator::BIT_OR:
            return crab::make_rc_mut<Integer>(lhs bitor rhs);
          case lexer::Operator::BIT_AND:
            return crab::make_rc_mut<Integer>(lhs bitand rhs);
          case lexer::Operator::EQUALS:
            return crab::make_rc_mut<Integer>(lhs == rhs);
          case lexer::Operator::NOT_EQUALS:
            return crab::make_rc_mut<Integer>(lhs != rhs);
          case lexer::Operator::GREATER:
            return crab::make_rc_mut<Integer>(lhs > rhs);
          case lexer::Operator::LESS:
            return crab::make_rc_mut<Integer>(lhs < rhs);
          case lexer::Operator::GREATER_OR_EQUALS:
            return crab::make_rc_mut<Integer>(lhs >= rhs);
          case lexer::Operator::LESS_OR_EQUALS:
            return crab::make_rc_mut<Integer>(lhs <= rhs);
          case lexer::Operator::SHIFT_LEFT:
            return crab::make_rc_mut<Integer>(lhs << rhs);
          case lexer::Operator::SHIFT_RIGHT:
            return crab::make_rc_mut<Integer>(lhs >> rhs);
          case lexer::Operator::ADD:
            return crab::make_rc_mut<Integer>(lhs + rhs);
          case lexer::Operator::SUB:
            return crab::make_rc_mut<Integer>(lhs - rhs);
          case lexer::Operator::MUL:
            return crab::make_rc_mut<Integer>(lhs * rhs);
          case lexer::Operator::DIV:
            return crab::make_rc_mut<Integer>(lhs / rhs);
          case lexer::Operator::MOD:
            return crab::make_rc_mut<Integer>(lhs % rhs);
          default: break;
        }
      }

      default: break;
    }

    return crab::make_rc_mut<Nil>();
  }

  auto Intepreter::visit_function_call(
    [[maybe_unused]] const ast::expression::FunctionCall &function_call
  ) -> RcMut<Value> {
    std::wcout << "funcall";
    // std::wcout << execute(function_call.get_function())->to_string();
    std::wcout << '(';
    std::wcout << str::join(
      function_call.get_arguments(),
      [this](auto &arg) {
        return execute(arg)->to_string();
      }
    );
    std::wcout << ')' << std::endl;

    return crab::make_rc_mut<Unit>();
  }

  auto Intepreter::visit_identifier_binding(
    [[maybe_unused]] const ast::expression::IdentifierBinding &identifier
  ) -> RcMut<Value> {
    throw std::logic_error("Not implemented: identifier_binding");
  }

  auto Intepreter::visit_unary([[maybe_unused]] const ast::expression::Unary &unary) -> RcMut<Value> {
    throw std::logic_error("Not implemented: unary");
  }

  auto Intepreter::visit_if([[maybe_unused]] const ast::expression::If &if_expr) -> RcMut<Value> {
    throw std::logic_error("Not implemented: if");
  }

  auto Intepreter::visit_scope([[maybe_unused]] const ast::expression::ScopeBlock &scope) -> RcMut<Value> {
    for (const auto &statement: scope.get_statements()) {
      execute(statement);
    }
    return execute(scope.get_eval());
  }

  auto Intepreter::visit_while([[maybe_unused]] const ast::expression::While &while_expr) -> RcMut<Value> {
    throw std::logic_error("Not implemented: while");
  }
}
