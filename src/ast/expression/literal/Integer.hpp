//
// Created by bishan_ on 4/26/24.
//

#pragma once
#include "ast/Expression.hpp"
#include "ast/Statement.hpp"

namespace goos::ast::expression {
  class Integer final : public Expression {
    i64 number;
    TokenTrace trace;

  public:
    explicit Integer(i64 number, TokenTrace trace) : number{number}, trace { trace } {}

    [[nodiscard]] auto get_number() const -> i64;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto token_trace() const -> TokenTrace override { return trace; }

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> override;
  };
} // namespace goos::ast::expression
