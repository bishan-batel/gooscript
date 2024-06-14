//
// Created by bishan_ on 5/3/24.
//

#pragma once

#include "box.hpp"
#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class Array final : public Expression {
    Vec<Box<Expression>> values;

  public:
    explicit Array(Vec<Box<Expression>> values);

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> RcMut<runtime::Value> override;
  };
}
