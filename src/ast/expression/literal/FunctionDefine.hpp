//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include <box.hpp>

#include "ast/Expression.hpp"
#include "meta/VariableDefinition.hpp"

namespace goos::ast::expression {
  class FunctionDefine final : public Expression {
    Option<String> name;
    Vec<meta::VariableDefinition> params;
    Box<Expression> body;

  public:
    explicit FunctionDefine(
      Option<String> name,
      Vec<meta::VariableDefinition> params,
      Box<Expression> body
    );

    [[nodiscard]] String to_string() const override;

    [[nodiscard]] Option<meta::VariantType> variant_type() const override;

    [[nodiscard]] const Vec<meta::VariableDefinition>& get_params() const;

    [[nodiscard]] const Expression& get_body() const;
  };
}
