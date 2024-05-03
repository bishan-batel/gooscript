//
// Created by bishan_ on 4/26/24.
//

#pragma once
#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class StringLiteral final : public Expression {
    String literal;

  public:
    explicit StringLiteral(String literal);

    [[nodiscard]] const String& get_string() const;

    [[nodiscard]] String to_string() const override;

    [[nodiscard]] Box<Expression> clone_expr() const override;
  };
}
