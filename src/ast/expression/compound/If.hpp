//
// Created by bishan_ on 5/1/24.
//

#pragma once
#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class If final : public Expression {
    Box<Expression> condition, then, else_then;

  public:
    If(Box<Expression> condition, Box<Expression> then, Box<Expression> else_then);

    [[nodiscard]] auto get_condition() const -> const Expression&;

    [[nodiscard]] auto get_then() const -> const Expression&;

    [[nodiscard]] auto get_else_then() const -> const Expression&;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> runtime::Result<runtime::Any> override;
  };
} // namespace goos::ast::expression
