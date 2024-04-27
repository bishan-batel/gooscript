//
// Created by bishan_ on 4/26/24.
//


#pragma once
#include "ast/Expression.hpp"

namespace goos::ast::expression {

class Unit final : public Expression {
public:
  Unit();

  [[nodiscard]] String to_string() const override;

  [[nodiscard]] Option<meta::VariantType> variant_type() const override;
};

}
