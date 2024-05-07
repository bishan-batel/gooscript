//
// Created by bishan_ on 4/26/24.
//


#pragma once
#include "ast/Expression.hpp"

namespace goos::ast::expression {

class Unit final : public Expression {
public:
  Unit();

  [[nodiscard]] WideString to_string() const override;

  [[nodiscard]] Box<Expression> clone_expr() const override;

  [[nodiscard]] bool operator==(const Statement &statement) const override;
};

}
