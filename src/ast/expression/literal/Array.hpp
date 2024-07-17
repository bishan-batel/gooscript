//
// Created by bishan_ on 5/3/24.
//

#pragma once

#include "ast/Expression.hpp"
#include "ast/Statement.hpp"
#include "box.hpp"

namespace goos::ast::expression {
  class Array final : public Expression {
    Vec<Box<Expression>> values;
    TokenTrace trace;

  public:
    explicit Array(Vec<Box<Expression>> values, TokenTrace trace) : values{std::move(values)}, trace{trace} {}

    [[nodiscard]] auto get_values() const -> const Vec<Box<Expression>> &;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> override;

    [[nodiscard]] auto token_trace() const -> TokenTrace override { return trace; }
  };
} // namespace goos::ast::expression
