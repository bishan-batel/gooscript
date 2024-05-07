//
// Created by bishan_ on 4/26/24.
//

#pragma once
#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class StringLiteral final : public Expression {
    WideString literal;

  public:
    explicit StringLiteral(WideString literal);

    [[nodiscard]] const WideString& get_string() const;

    [[nodiscard]] WideString to_string() const override;

    [[nodiscard]] Box<Expression> clone_expr() const override;

    [[nodiscard]] bool operator==(const Statement &statement) const override;
  };
}
