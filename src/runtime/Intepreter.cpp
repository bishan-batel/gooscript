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

#include "ast/expression/Binary.hpp"
#include "ast/expression/FunctionCall.hpp"
#include "ast/expression/compound/ScopeBlock.hpp"
#include "ast/expression/literal/Boolean.hpp"
#include "ast/expression/literal/Integer.hpp"
#include "data/Boolean.hpp"
#include "data/Integer.hpp"
#include "data/Nil.hpp"
#include "data/Unit.hpp"
#include "data/Value.hpp"
#include "utils/str.hpp"

namespace goos::runtime {
  auto Intepreter::execute(const ast::Statement &statement) -> void {
    statement.accept(*this);
  }

  auto Intepreter::execute(const ast::Expression &expr) -> std::shared_ptr<Value> {
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
    throw std::logic_error("Not implemented: variable_declaration");
  }

  auto Intepreter::visit_array([[maybe_unused]] const ast::expression::Array &array) -> std::shared_ptr<Value> {
    throw std::logic_error("Not implemented: array");
  }

  auto Intepreter::visit_boolean([[maybe_unused]] const ast::expression::Boolean &boolean) -> std::shared_ptr<Value> {
    return std::make_shared<Boolean>(boolean.get_state());
  }

  auto Intepreter::visit_decimal([[maybe_unused]] const ast::expression::Decimal &decimal) -> std::shared_ptr<Value> {
    throw std::logic_error("Not implemented: decimal");
  }

  auto Intepreter::visit_dictionary(
    [[maybe_unused]] const ast::expression::Dictionary &dictionary
  ) -> std::shared_ptr<Value> {
    throw std::logic_error("Not implemented: dictionary");
  }

  auto Intepreter::visit_integer([[maybe_unused]] const ast::expression::Integer &integer) -> std::shared_ptr<Value> {
    return std::make_shared<Integer>(integer.get_number());
  }

  auto Intepreter::visit_lambda([[maybe_unused]] const ast::expression::Lambda &lambda) -> std::shared_ptr<Value> {
    throw std::logic_error("Not implemented: lambda");
  }

  auto Intepreter::visit_nil([[maybe_unused]] const ast::expression::Nil &nil) -> std::shared_ptr<Value> {
    return std::make_shared<Nil>();
  }

  auto Intepreter::visit_string_literal(
    [[maybe_unused]] const ast::expression::StringLiteral &str
  ) -> std::shared_ptr<Value> {
    throw std::logic_error("Not implemented: string_literal");
  }

  auto Intepreter::visit_unit([[maybe_unused]] const ast::expression::Unit &unit) -> std::shared_ptr<Value> {
    return std::make_shared<Unit>();
  }

  auto Intepreter::visit_binary([[maybe_unused]] const ast::expression::Binary &binary) -> std::shared_ptr<Value> {
    const auto lhs_val{execute(binary.get_lhs())};
    const auto rhs_val{execute(binary.get_rhs())};

    if (lhs_val->get_type() != rhs_val->get_type()) return std::make_shared<Nil>();

    switch (lhs_val->get_type()) {
      case meta::VariantType::INTEGER: {
        const auto lhs = lhs_val->coerce_unchecked<Integer>().get();
        const auto rhs = rhs_val->coerce_unchecked<Integer>().get();

        switch (binary.get_op()) {
          case lexer::Operator::XOR:
            return std::make_shared<Integer>(lhs xor rhs);
          case lexer::Operator::BIT_OR:
            return std::make_shared<Integer>(lhs bitor rhs);
          case lexer::Operator::BIT_AND:
            return std::make_shared<Integer>(lhs bitand rhs);
          case lexer::Operator::EQUALS:
            return std::make_shared<Integer>(lhs == rhs);
          case lexer::Operator::NOT_EQUALS:
            return std::make_shared<Integer>(lhs != rhs);
          case lexer::Operator::GREATER:
            return std::make_shared<Integer>(lhs > rhs);
          case lexer::Operator::LESS:
            return std::make_shared<Integer>(lhs < rhs);
          case lexer::Operator::GREATER_OR_EQUALS:
            return std::make_shared<Integer>(lhs >= rhs);
          case lexer::Operator::LESS_OR_EQUALS:
            return std::make_shared<Integer>(lhs <= rhs);
          case lexer::Operator::SHIFT_LEFT:
            return std::make_shared<Integer>(lhs << rhs);
          case lexer::Operator::SHIFT_RIGHT:
            return std::make_shared<Integer>(lhs >> rhs);
          case lexer::Operator::ADD:
            return std::make_shared<Integer>(lhs + rhs);
          case lexer::Operator::SUB:
            return std::make_shared<Integer>(lhs - rhs);
          case lexer::Operator::MUL:
            return std::make_shared<Integer>(lhs * rhs);
          case lexer::Operator::DIV:
            return std::make_shared<Integer>(lhs / rhs);
          case lexer::Operator::MOD:
            return std::make_shared<Integer>(lhs % rhs);
          default: break;
        }
      }

      default: break;
    }

    return std::make_shared<Nil>();
  }

  auto Intepreter::visit_function_call(
    [[maybe_unused]] const ast::expression::FunctionCall &function_call
  ) -> std::shared_ptr<Value> {
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

    return std::make_shared<Unit>();
  }

  auto Intepreter::visit_identifier_binding(
    [[maybe_unused]] const ast::expression::IdentifierBinding &identifier
  ) -> std::shared_ptr<Value> {
    throw std::logic_error("Not implemented: identifier_binding");
  }

  auto Intepreter::visit_unary([[maybe_unused]] const ast::expression::Unary &unary) -> std::shared_ptr<Value> {
    throw std::logic_error("Not implemented: unary");
  }

  auto Intepreter::visit_if([[maybe_unused]] const ast::expression::If &if_expr) -> std::shared_ptr<Value> {
    throw std::logic_error("Not implemented: if");
  }

  auto Intepreter::visit_scope([[maybe_unused]] const ast::expression::ScopeBlock &scope) -> std::shared_ptr<Value> {
    for (const auto &statement: scope.get_statements()) {
      execute(statement);
    }
    return execute(scope.get_eval());
  }

  auto Intepreter::visit_while([[maybe_unused]] const ast::expression::While &while_expr) -> std::shared_ptr<Value> {
    throw std::logic_error("Not implemented: while");
  }
}
