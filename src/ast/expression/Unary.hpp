//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include <box.hpp>
#include "lexer/Operator.hpp"
#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class Unary final : public Expression {
    Box<Expression> expr;
    lexer::Operator op;

  public:
    Unary(lexer::Operator op, Box<Expression> expr);

    [[nodiscard]] const Expression& get_expression() const;

    [[nodiscard]] WideString to_string() const override;

    [[nodiscard]] Box<Expression> clone_expr() const override;

    [[nodiscard]] bool operator==(const Statement &statement) const override;
  };
}
