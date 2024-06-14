//
// Created bany bish_ on 5/15/24.
//

#pragma once
#include "ast/Expression.hpp"
#include "ast/Statement.hpp"

namespace goos::runtime {
  class Intepreter final : ast::Statement::IVisitor {
  public:
    auto execute(const ast::Statement &statement) -> void;

    auto execute(const ast::Expression &expr) -> RcMut<Value>;

    auto visit_eval(const ast::Eval &eval) -> void override;

    auto visit_return(const ast::Return &ret) -> void override;

    auto visit_variable_declaration(const ast::VariableDeclaration &variable_declaration) -> void override;

    auto visit_array(const ast::expression::Array &array) -> RcMut<Value> override;

    auto visit_boolean(const ast::expression::Boolean &boolean) -> RcMut<Value> override;

    auto visit_decimal(const ast::expression::Decimal &decimal) -> RcMut<Value> override;

    auto visit_dictionary(const ast::expression::Dictionary &dictionary) -> RcMut<Value> override;

    auto visit_integer(const ast::expression::Integer &integer) -> RcMut<Value> override;

    auto visit_lambda(const ast::expression::Lambda &lambda) -> RcMut<Value> override;

    auto visit_nil(const ast::expression::Nil &nil) -> RcMut<Value> override;

    auto visit_string_literal(const ast::expression::StringLiteral &str) -> RcMut<Value> override;

    auto visit_unit(const ast::expression::Unit &unit) -> RcMut<Value> override;

    auto visit_binary(const ast::expression::Binary &binary) -> RcMut<Value> override;

    auto visit_function_call(
      const ast::expression::FunctionCall &function_call
    ) -> RcMut<Value> override;

    auto visit_identifier_binding(
      const ast::expression::IdentifierBinding &identifier
    ) -> RcMut<Value> override;

    auto visit_unary(const ast::expression::Unary &unary) -> RcMut<Value> override;

    auto visit_if(const ast::expression::If &if_expr) -> RcMut<Value> override;

    auto visit_scope(const ast::expression::ScopeBlock &scope) -> RcMut<Value> override;

    auto visit_while(const ast::expression::While &while_expr) -> RcMut<Value> override;
  };
}
