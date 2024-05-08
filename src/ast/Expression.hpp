//
// Created by bishan_ on 4/25/24.
//

#pragma once

#include "Statement.hpp"

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
  };
}
