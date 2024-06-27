//
// Created by bishan_ on 5/2/24.
//

#pragma once
#include <preamble.hpp>

#include "ast/Expression.hpp"
#include "meta/Identifier.hpp"

namespace goos::ast::expression {
  class IdentifierBinding final : public Expression {
    meta::Identifier identifier;

  public:
    explicit IdentifierBinding(meta::Identifier identifier);

    [[nodiscard]] auto get_identifier() const -> const meta::Identifier&;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> override;
  };
}
