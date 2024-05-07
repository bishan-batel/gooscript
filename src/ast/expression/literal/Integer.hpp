//
// Created by bishan_ on 4/26/24.
//

#pragma once
#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class Integer final : public Expression {
    i64 number;

  public:
    explicit Integer(i64 number);

    [[nodiscard]] WideString to_string() const override;

    [[nodiscard]] Box<Expression> clone_expr() const override;
  };
}
