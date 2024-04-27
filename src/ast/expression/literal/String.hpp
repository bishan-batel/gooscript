//
// Created by bishan_ on 4/26/24.
//

#pragma once
#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class String final : public Expression {
    ::String literal;

  public:
    explicit String(::String literal);

    [[nodiscard]] const ::String& get_string() const;

    [[nodiscard]] ::String to_string() const override;

    [[nodiscard]] Option<meta::VariantType> variant_type() const override;
  };
}
