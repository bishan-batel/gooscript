//
// Created by bishan_ on 4/27/24.
//

#pragma once
#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class Nil final : public Expression {
  public:

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> std::shared_ptr<runtime::Value> override;
  };
} // goos
