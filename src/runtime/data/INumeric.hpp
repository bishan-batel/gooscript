//
// Created by bishan_ on 6/15/24.
//

#pragma once
#include <preamble.hpp>

#include "IValue.hpp"

namespace goos::runtime {
  struct INumeric : IValue {
    virtual auto set_number(f64 v) -> void = 0;

    [[nodiscard]] virtual auto get_number() const -> f64 = 0;

    [[nodiscard]] virtual auto is_integral() const -> bool = 0;
  };
}
