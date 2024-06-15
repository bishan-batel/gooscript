//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include <option.hpp>

#include "Value.hpp"

namespace goos::runtime {
  enum class ArityType {
    Varargs,
    Finite
  };

  class Callable : public Value {
  public:
    [[nodiscard]] virtual auto call(const Vec<Any> &values) const -> Result<Any> = 0;

    [[nodiscard]] virtual auto get_arity_type() const -> ArityType = 0;

    [[nodiscard]] virtual auto get_arity() const -> usize = 0;
  };
}
