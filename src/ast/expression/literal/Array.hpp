//
// Created by bishan_ on 5/3/24.
//

#pragma once

#include "box.hpp"
#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class Array final : public Expression {
    Vec<Box<Expression>> values;

  public:
    explicit Array(Vec<Box<Expression>> values);

    [[nodiscard]] String to_string() const override;

    [[nodiscard]] Box<Expression> clone_expr() const override;
  };
}
