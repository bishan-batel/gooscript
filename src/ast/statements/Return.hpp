//
// Created by bishan_ on 5/1/24.
//

#pragma once
#include <box.hpp>
#include "ast/Expression.hpp"
#include "ast/Statement.hpp"

namespace goos::ast {
  class Return final : public Statement {
    Box<Expression> expression;
    TokenTrace trace;

  public:
    explicit Return(Box<Expression> expr, TokenTrace trace) : expression{std::move(expr)}, trace{trace} {}

    [[nodiscard]] auto get_expression() const -> const Expression &;
    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone() const -> Box<Statement> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    auto accept(IVisitor &visitor) const -> Result<unit, Box<crab::Error>> override;

    [[nodiscard]] auto token_trace() const -> TokenTrace override { return trace; }
  };
} // namespace goos::ast
