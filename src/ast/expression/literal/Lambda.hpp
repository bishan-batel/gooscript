//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include <box.hpp>

#include "ast/Expression.hpp"
#include "meta/VariableDefinition.hpp"

namespace goos::ast::expression {
  class Lambda final : public Expression {
    Vec<String> params;
    Box<Expression> body;

  public:
    explicit Lambda(
      Vec<String> params,
      Box<Expression> body
    );

    [[nodiscard]] String to_string() const override;

    [[nodiscard]] const Vec<String>& get_params() const;

    [[nodiscard]] const Expression& get_body() const;

    [[nodiscard]] Box<Expression> clone_expr() const override;
  };
}
