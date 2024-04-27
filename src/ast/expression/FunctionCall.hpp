//
// Created by bishan_ on 4/26/24.
//

#pragma once
#include <ast/Expression.hpp>
#include <box.hpp>

namespace goos::ast::expression {
  class FunctionCall final : public Expression {
    Box<Expression> function;
    Vec<Box<Expression>> arguments;

  public:
    FunctionCall(Box<Expression> function, Vec<Box<Expression>> arguments);

    [[nodiscard]] String to_string() const override;

    [[nodiscard]] Option<meta::VariantType> variant_type() const override;
  };
}
