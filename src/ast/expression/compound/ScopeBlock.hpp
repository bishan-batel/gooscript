//
// Created by bishan_ on 4/26/24.
//

#pragma once
#include <box.hpp>

#include "ast/Expression.hpp"
#include "ast/statements/Eval.hpp"

namespace goos::ast::expression {
  class ScopeBlock final : public Expression {
    Vec<Box<Statement>> statements;
    Box<Expression> eval;

  public:
    explicit ScopeBlock(Vec<Box<Statement>> statements);

    explicit ScopeBlock(Vec<Box<Statement>> statements, Box<Expression> eval);

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> runtime::Result<runtime::Any> override;

    [[nodiscard]] auto get_statements() const -> const Vec<Box<Statement>>&;

    [[nodiscard]] auto get_eval() const -> const Expression&;
  };
}
