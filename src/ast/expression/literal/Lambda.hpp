//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include <box.hpp>

#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class Lambda final : public Expression {
    Vec<WideString> params;
    Box<Expression> body;

  public:
    explicit Lambda(
      Vec<WideString> params,
      Box<Expression> body
    );

    [[nodiscard]] WideString to_string() const override;

    [[nodiscard]] const Vec<WideString>& get_params() const;

    [[nodiscard]] const Expression& get_body() const;

    [[nodiscard]] Box<Expression> clone_expr() const override;

    [[nodiscard]] bool operator==(const Statement &statement) const override;
  };
}
