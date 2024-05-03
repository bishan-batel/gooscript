//
// Created by bishan_ on 5/2/24.
//

#pragma once

#include <box.hpp>

#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class While final : public Expression {
    Box<Expression> condition, body;

  public:
    While(Box<Expression> condition, Box<Expression> body);

    [[nodiscard]] const Expression& get_condition() const;

    [[nodiscard]] const Expression& get_body() const;

    [[nodiscard]] String to_string() const override;

    [[nodiscard]] Box<Expression> clone_expr() const override;
  };
} // namespace goos::ast::expression
