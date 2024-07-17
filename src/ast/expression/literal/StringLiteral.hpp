//
// Created by bishan_ on 4/26/24.
//

#pragma once
#include "ast/Expression.hpp"
#include "ast/Statement.hpp"

namespace goos::ast::expression {
  class StringLiteral final : public Expression {
    Rc<WideString> literal;
    TokenTrace trace;

  public:
    explicit StringLiteral(Rc<WideString> literal, TokenTrace trace) : literal{literal}, trace{trace} {}

    [[nodiscard]] auto get_string() const -> Rc<WideString>;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> override;

    [[nodiscard]] auto token_trace() const -> TokenTrace override { return trace; }
  };
} // namespace goos::ast::expression
