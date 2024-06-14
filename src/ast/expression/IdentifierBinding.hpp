//
// Created by bishan_ on 5/2/24.
//

#pragma once
#include <preamble.hpp>

#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class IdentifierBinding final : public Expression {
    WideString identifier;

  public:
    explicit IdentifierBinding(WideString identifier);

    [[nodiscard]] auto get_identifier() const -> const WideString&;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> RcMut<runtime::Value> override;
  };
}
