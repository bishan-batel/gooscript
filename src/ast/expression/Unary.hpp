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

    [[nodiscard]] auto get_expression() const -> const Expression&;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;
  };
}
