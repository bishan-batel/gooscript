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

    [[nodiscard]] WideString to_string() const override;

    [[nodiscard]] Box<Expression> clone_expr() const override;

    [[nodiscard]] bool operator==(const Statement &statement) const override;
  };
}
