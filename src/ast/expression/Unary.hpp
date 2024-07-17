//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include <box.hpp>
#include "ast/Expression.hpp"
#include "ast/Statement.hpp"
#include "lexer/Operator.hpp"

namespace goos::ast::expression {
  class Unary final : public Expression {
    Box<Expression> expr;
    lexer::Operator op;
    TokenTrace op_trace;

  public:
    Unary(lexer::Operator op, Box<Expression> expr, TokenTrace op_trace)
        : expr{std::move(expr)}, op{op}, op_trace(op_trace) {}

    [[nodiscard]] auto get_expression() const -> const Expression &;

    [[nodiscard]] auto get_op() const -> lexer::Operator;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> override;

    [[nodiscard]] auto token_trace() const -> TokenTrace override;
  };
} // namespace goos::ast::expression
