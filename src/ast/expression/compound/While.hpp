//
// Created by bishan_ on 5/2/24.
//

#pragma once

#include <box.hpp>

#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class While final : public Expression {
    Box<Expression> condition, body;
    TokenTrace trace;

  public:
    While(Box<Expression> condition, Box<Expression> body, TokenTrace trace)
      : condition{std::move(condition)}, body{std::move(body)}, trace{trace} {}

    [[nodiscard]] auto get_condition() const -> const Expression&;

    [[nodiscard]] auto get_body() const -> const Expression&;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> override;

    [[nodiscard]] auto token_trace() const -> TokenTrace override { return trace; }
  };
} // namespace goos::ast::expression
