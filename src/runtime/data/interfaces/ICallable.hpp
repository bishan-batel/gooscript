//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include <option.hpp>

#include "runtime/Environment.hpp"

namespace goos::runtime {
  class Intepreter;

  enum class ArityType {
    Varargs,
    Finite
  };

  struct ICallable : IValue {
    static constexpr auto TYPE = meta::VariantType::FUNCTION;

    [[nodiscard]] virtual auto call(Intepreter &runtime, const Vec<Any> &values) const -> Result<Any> = 0;

    [[nodiscard]] virtual auto get_arity_type() const -> ArityType = 0;

    [[nodiscard]] virtual auto get_arity() const -> usize = 0;
  };
}
