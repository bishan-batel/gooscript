//
// Created by bishan_ on 4/26/24.
//

#pragma once
#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class Boolean final : public Expression {
    bool state;

  public:
    explicit Boolean(const bool state)
      : state{state} {}

    [[nodiscard]] auto get_state() const -> bool;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> runtime::Result<runtime::Any> override;
  };
}
