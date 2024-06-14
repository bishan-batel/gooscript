//
// Created by bishan_ on 5/2/24.
//

#pragma once

#include <box.hpp>

#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class While final : public Expression {
    Box<Expression> condition, body;

  public:
    While(Box<Expression> condition, Box<Expression> body);

    [[nodiscard]] auto get_condition() const -> const Expression&;

    [[nodiscard]] auto get_body() const -> const Expression&;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> runtime::Result<runtime::Any> override;
  };
} // namespace goos::ast::expression
