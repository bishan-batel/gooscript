//
// Created by bishan_ on 6/15/24.
//

#pragma once
#include "ast/Expression.hpp"
#include "meta/Identifier.hpp"

namespace goos::ast::expression {
  class PropertyAccess final : public Expression {
    Box<Expression> object;
    meta::Identifier property;

  public:
    PropertyAccess(Box<Expression> object, meta::Identifier property);

    [[nodiscard]] auto get_object() const -> const Expression&;

    [[nodiscard]] auto get_property() const -> meta::Identifier;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> override;
  };
}
