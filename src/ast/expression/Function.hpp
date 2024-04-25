//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include <ast/Expression.hpp>
#include <build/Release/_deps/crab-src/include/box.hpp>
#include <meta/VariableDefinition.hpp>

namespace goos::ast::expression {
  class Function final : public Expression {
    Option<String> name;
    Vec<meta::VariableDefinition> params;
    Box<Expression> body;

  public:
    explicit Function(
      Option<String> name,
      Vec<meta::VariableDefinition> params,
      Box<Expression> body
    );

    [[nodiscard]] const Vec<meta::VariableDefinition> &get_params() const;

    [[nodiscard]] const Expression &get_body() const;

    [[nodiscard]] String to_string() const override;

    Option<meta::VariantType> variant_type() override;
  };
}
