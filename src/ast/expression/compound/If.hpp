//
// Created by bishan_ on 5/1/24.
//

#pragma once
#include "ast/Expression.hpp"

namespace goos ::ast ::expression {
  class If final : public Expression {
    Box<Expression> condition, then, else_then;

  public:
    If(Box<Expression> condition, Box<Expression> then, Box<Expression> else_then);

    [[nodiscard]] WideString to_string() const override;

    [[nodiscard]] Box<Expression> clone_expr() const override;

    [[nodiscard]] bool operator==(const Statement &statement) const override;
  };
} // namespace goos::ast::expression
