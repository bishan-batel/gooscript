//
// Created by bishan_ on 4/25/24.
//

#pragma once

#include "Statement.hpp"

namespace goos::ast {
  class Expression : public Statement {
  public:
    Expression() = default;

    [[nodiscard]] virtual auto clone_expr() const -> Box<Expression> = 0;

    [[nodiscard]] auto clone() const -> Box<Statement> final;
  };
}
