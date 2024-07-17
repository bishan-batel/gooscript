//
// Created by bishan_ on 4/27/24.
//

#pragma once
#include "ast/Expression.hpp"
#include "ast/Statement.hpp"

namespace goos::ast::expression {
  class Nil final : public Expression {
    TokenTrace trace;

  public:
    explicit Nil(TokenTrace trace) : trace{trace} {}

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;


    [[nodiscard]] auto token_trace() const -> TokenTrace override { return trace; }

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> override;
  };
} // namespace goos::ast::expression
