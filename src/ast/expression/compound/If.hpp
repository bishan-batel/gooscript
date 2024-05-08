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

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;
  };
} // namespace goos::ast::expression
