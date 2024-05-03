//
// Created by bishan_ on 4/27/24.
//

#pragma once
#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class Nil final : public Expression {
  public:
    Nil();

    [[nodiscard]] String to_string() const override;

    [[nodiscard]] Box<Expression> clone_expr() const override;
  };
} // goos
