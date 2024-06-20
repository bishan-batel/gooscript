//
// Created by bishan_ on 6/15/24.
//

#pragma once
#include <preamble.hpp>

#include "IValue.hpp"

namespace goos::runtime {
  struct INumeric : IValue {
    [[nodiscard]] virtual auto get_number() const -> f64 = 0;

    [[nodiscard]] virtual auto is_integral() const -> bool = 0;

    [[nodiscard]] auto operator*() const -> f64 {
      return get_number();
    }
  };
}
