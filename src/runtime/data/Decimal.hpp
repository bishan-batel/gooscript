//
// Created by bishan_ on 5/16/24.
//

#pragma once
#include "Value.hpp"

namespace goos::runtime {
  class Decimal final : public Value {
    f64 value;

  public:
    explicit Decimal(f64 value);

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;
  };
}
