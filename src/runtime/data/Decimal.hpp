//
// Created by bishan_ on 5/16/24.
//

#pragma once
#include "Value.hpp"

namespace goos::runtime {
  class Decimal final : public Value {
    f64 value;

  public:
    using Contained = f64;

    explicit Decimal(f64 value = 0.f);

    [[nodiscard]] auto get() const -> f64;

    auto set(f64 v) -> void;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;
  };
}
