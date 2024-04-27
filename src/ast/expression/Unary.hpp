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

    [[nodiscard]] String to_string() const override;

    [[nodiscard]] Option<meta::VariantType> variant_type() const override;
  };
}
