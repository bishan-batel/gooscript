//
// Created by bishan_ on 4/25/24.
//

#pragma once

#include "Statement.hpp"

namespace goos::ast::expression {
  class ScopeBlock;
  class While;
  class If;
  class Unary;
  class IdentifierBinding;
  class FunctionCall;
  class Binary;
  class Unit;
  class StringLiteral;
  class Nil;
  class Lambda;
  class Integer;
  class Dictionary;
  class Decimal;
  class Boolean;
  class Array;
}

namespace goos::ast {
  class VariableDeclaration;
  class Return;
  class Eval;

  class Expression : public Statement {
  public:
    struct IVisitor {
      virtual ~IVisitor() = default;

      virtual auto visit_array(const expression::Array &array) -> std::shared_ptr<runtime::Value> = 0;

      virtual auto visit_boolean(const expression::Boolean &boolean) -> std::shared_ptr<runtime::Value> = 0;

      virtual auto visit_decimal(const expression::Decimal &decimal) -> std::shared_ptr<runtime::Value> = 0;

      virtual auto visit_dictionary(const expression::Dictionary &dictionary) -> std::shared_ptr<runtime::Value> = 0;

      virtual auto visit_integer(const expression::Integer &integer) -> std::shared_ptr<runtime::Value> = 0;

      virtual auto visit_lambda(const expression::Lambda &lambda) -> std::shared_ptr<runtime::Value> = 0;

      virtual auto visit_nil(const expression::Nil &nil) -> std::shared_ptr<runtime::Value> = 0;

      virtual auto visit_string_literal(const expression::StringLiteral &str) -> std::shared_ptr<runtime::Value> = 0;

      virtual auto visit_unit(const expression::Unit &unit) -> std::shared_ptr<runtime::Value> = 0;

      virtual auto visit_binary(const expression::Binary &binary) -> std::shared_ptr<runtime::Value> = 0;

      virtual auto visit_function_call(
        const expression::FunctionCall &function_call
      ) -> std::shared_ptr<runtime::Value> = 0;

      virtual auto visit_identifier_binding(
        const expression::IdentifierBinding &identifier
      ) -> std::shared_ptr<runtime::Value> = 0;

      virtual auto visit_unary(const expression::Unary &unary) -> std::shared_ptr<runtime::Value> = 0;

      virtual auto visit_if(const expression::If &if_expr) -> std::shared_ptr<runtime::Value> = 0;

      virtual auto visit_scope(const expression::ScopeBlock &scope) -> std::shared_ptr<runtime::Value> = 0;

      virtual auto visit_while(const expression::While &while_expr) -> std::shared_ptr<runtime::Value> = 0;
    };

    Expression() = default;

    Expression(const Expression &) = delete;

    Expression(Expression &&) = default;

    auto operator=(const Expression &) -> Expression& = delete;

    auto operator=(Expression &&) -> Expression& = default;

    ~Expression() override = default;

    [[nodiscard]] virtual auto clone_expr() const -> Box<Expression> = 0;

    [[nodiscard]] auto clone() const -> Box<Statement> final;

    // ReSharper disable once CppFinalNonOverridingVirtualFunction
    auto accept(Statement::IVisitor &visitor) const -> void final;

    [[nodiscard]] virtual auto accept_expr(IVisitor &visitor) const -> std::shared_ptr<runtime::Value> = 0;
  };
}
