//
// Created by bishan_ on 4/26/24.
//

#pragma once
#include "ast/Expression.hpp"
#include "ast/Statement.hpp"

namespace goos::ast::expression {
  class Boolean final : public Expression {
    bool state;
    TokenTrace trace;

  public:
    explicit Boolean(bool state, TokenTrace trace) : state{state}, trace{trace} {}

    [[nodiscard]] auto get_state() const -> bool;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto token_trace() const -> TokenTrace override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> override;
  };
} // namespace goos::ast::expression
