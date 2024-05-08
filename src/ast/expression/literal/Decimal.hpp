//
// Created by bishan_ on 4/26/24.
//

#pragma once
#include "ast/Expression.hpp"

namespace goos ::ast:: expression {
  class Decimal final : public Expression {
    f64 literal;

  public:
    explicit Decimal(f64 literal);

    [[nodiscard]] auto get_number() const -> f64;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;
  };
}
