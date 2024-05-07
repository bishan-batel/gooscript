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

    [[nodiscard]] auto get_lhs() const -> const Expression&;

    [[nodiscard]] auto get_rhs() const -> const Expression&;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;
  };
}
