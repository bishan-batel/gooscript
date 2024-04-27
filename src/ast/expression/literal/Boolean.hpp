//
// Created by bishan_ on 4/26/24.
//

#pragma once
#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class Boolean final : public Expression {
    bool state;

  public:
    explicit Boolean(const bool state)
      : state{state} {}

    [[nodiscard]] bool get_state() const;

    [[nodiscard]] String to_string() const override;

    [[nodiscard]] Option<meta::VariantType> variant_type() const override;
  };
}
