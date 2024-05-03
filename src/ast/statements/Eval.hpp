//
// Created by bishan_ on 4/26/24.
//

#pragma once
#include <box.hpp>

#include "ast/Expression.hpp"
#include "ast/Statement.hpp"

namespace goos::ast {
  class Eval final : public Statement {
    Box<Expression> expr;

  public:
    explicit Eval(Box<Expression> expr);

    [[nodiscard]] const Expression& get_expr() const;

    [[nodiscard]] String to_string() const override;
  };
}
