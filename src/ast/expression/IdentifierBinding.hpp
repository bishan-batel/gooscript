//
// Created by bishan_ on 5/2/24.
//

#pragma once
#include <preamble.hpp>

#include "ast/Expression.hpp"
#include "ast/Statement.hpp"
#include "meta/Identifier.hpp"

namespace goos::ast::expression {
  class IdentifierBinding final : public Expression {
    meta::Identifier identifier;
    TokenTrace trace;

  public:
    explicit IdentifierBinding(meta::Identifier identifier, TokenTrace trace): identifier{identifier}, trace{trace} {}

    [[nodiscard]] auto get_identifier() const -> const meta::Identifier&;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> override;

    [[nodiscard]] auto token_trace() const -> TokenTrace override { return trace; }
  };
}
