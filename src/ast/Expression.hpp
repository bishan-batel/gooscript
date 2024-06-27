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
  class Expression : public Statement {
  public:
    Expression() = default;

    Expression(const Expression &) = delete;

    Expression(Expression &&) = default;

    auto operator=(const Expression &) -> Expression& = delete;

    auto operator=(Expression &&) -> Expression& = default;

    ~Expression() override = default;

    [[nodiscard]] virtual auto clone_expr() const -> Box<Expression> = 0;

    [[nodiscard]] auto clone() const -> Box<Statement> final;

    // ReSharper disable once CppFinalNonOverridingVirtualFunction
    auto accept(IVisitor &visitor) const -> Result<unit, Box<crab::Error>> final;

    [[nodiscard]] virtual auto accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> = 0;
  };
}
