//
// Created by bishan_ on 4/26/24.
//

#pragma once
#include <box.hpp>

#include "ast/Expression.hpp"
#include "ast/Statement.hpp"
#include "ast/expression/literal/Unit.hpp"

namespace goos::ast::expression {
  class ScopeBlock final : public Expression {
    Vec<Box<Statement>> statements;
    Box<Expression> eval;
    TokenTrace trace;

  public:
    explicit ScopeBlock(Vec<Box<Statement>> statements, TokenTrace trace)
        : statements{std::move(statements)}, eval{crab::make_box<Unit>(trace)}, trace{trace} {}

    explicit ScopeBlock(Vec<Box<Statement>> statements, Box<Expression> eval, TokenTrace trace)
        : statements{std::move(statements)}, eval{std::move(eval)}, trace{trace} {}

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> override;

    [[nodiscard]] auto get_statements() const -> const Vec<Box<Statement>> &;

    [[nodiscard]] auto get_eval() const -> const Expression &;

    [[nodiscard]] auto token_trace() const -> TokenTrace override { return trace; }
  };
} // namespace goos::ast::expression
