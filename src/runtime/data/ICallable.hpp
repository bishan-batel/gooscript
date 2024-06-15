//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include <option.hpp>

#include "IValue.hpp"
#include "runtime/Environment.hpp"

namespace goos::runtime {
  enum class ArityType {
    Varargs,
    Finite
  };

  class ICallable : public IValue {
  public:
    [[nodiscard]] virtual auto call(Environment &env, const Vec<Any> &values) const -> Result<Any> = 0;

    [[nodiscard]] virtual auto get_arity_type() const -> ArityType = 0;

    [[nodiscard]] virtual auto get_arity() const -> usize = 0;
  };
}
