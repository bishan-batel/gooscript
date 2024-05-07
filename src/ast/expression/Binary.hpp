//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include "ast/Expression.hpp"
#include <box.hpp>
#include <lexer/Operator.hpp>

namespace goos::ast::expression {
  class Binary final : public Expression {
    Box<Expression> lhs, rhs;
    lexer::Operator op;

  public:
    Binary(Box<Expression> lhs, lexer::Operator op, Box<Expression> rhs);

    [[nodiscard]] const Expression& get_lhs() const;

    [[nodiscard]] const Expression& get_rhs() const;

    [[nodiscard]] WideString to_string() const override;

    [[nodiscard]] Box<Expression> clone_expr() const override;

    [[nodiscard]] bool operator==(const Statement &statement) const override;
  };
}
