//
// Created by bishan_ on 5/1/24.
//

#pragma once
#include <box.hpp>
#include "ast/Expression.hpp"

namespace goos ::ast ::expression {
  class If final: public Expression {
      Box<Expression> condition, then, else_then;

    public:
      If(Box<Expression> condition, Box<Expression> then, Box<Expression> else_then);

      [[nodiscard]] String to_string() const override;

      [[nodiscard]] Option<meta::VariantType> variant_type() const override;
  };
} // namespace goos::ast::expression
