//
// Created by bishan_ on 5/15/24.
//

#pragma once
#include "ast/Expression.hpp"
#include "ast/Statement.hpp"

namespace goos::runtime {
  class Intepreter final : ast::Statement::IVisitor, ast::Expression::IVisitor {
  public:
    auto execute(const ast::Statement &statement) -> void;

    auto execute(const ast::Expression &expr) -> std::shared_ptr<Value>;

    auto visit_eval(const ast::Eval &eval) -> void override;

    auto visit_return(const ast::Return &ret) -> void override;

    auto visit_variable_declaration(const ast::VariableDeclaration &variable_declaration) -> void override;

    auto visit_array(const ast::expression::Array &array) -> std::shared_ptr<Value> override;

    auto visit_boolean(const ast::expression::Boolean &boolean) -> std::shared_ptr<Value> override;

    auto visit_decimal(const ast::expression::Decimal &decimal) -> std::shared_ptr<Value> override;

    auto visit_dictionary(const ast::expression::Dictionary &dictionary) -> std::shared_ptr<Value> override;

    auto visit_integer(const ast::expression::Integer &integer) -> std::shared_ptr<Value> override;

    auto visit_lambda(const ast::expression::Lambda &lambda) -> std::shared_ptr<Value> override;

    auto visit_nil(const ast::expression::Nil &nil) -> std::shared_ptr<Value> override;

    auto visit_string_literal(const ast::expression::StringLiteral &str) -> std::shared_ptr<Value> override;

    auto visit_unit(const ast::expression::Unit &unit) -> std::shared_ptr<Value> override;

    auto visit_binary(const ast::expression::Binary &binary) -> std::shared_ptr<Value> override;

    auto visit_function_call(
      const ast::expression::FunctionCall &function_call
    ) -> std::shared_ptr<Value> override;

    auto visit_identifier_binding(
      const ast::expression::IdentifierBinding &identifier
    ) -> std::shared_ptr<Value> override;

    auto visit_unary(const ast::expression::Unary &unary) -> std::shared_ptr<Value> override;

    auto visit_if(const ast::expression::If &if_expr) -> std::shared_ptr<Value> override;

    auto visit_scope(const ast::expression::ScopeBlock &scope) -> std::shared_ptr<Value> override;

    auto visit_while(const ast::expression::While &while_expr) -> std::shared_ptr<Value> override;
  };
}
