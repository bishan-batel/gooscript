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

    [[nodiscard]] f64 get_number() const;

    [[nodiscard]] WideString to_string() const override;

    [[nodiscard]] Box<Expression> clone_expr() const override;
  };
}
