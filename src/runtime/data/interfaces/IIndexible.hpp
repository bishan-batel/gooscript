//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include <option.hpp>
#include <utility>

#include "IValue.hpp"
#include "runtime/err/RuntimeError.hpp"

namespace goos::runtime {
  struct IIndexible : IValue {
    static constexpr auto TYPE = meta::VariantType::FUNCTION;

    [[nodiscard]] virtual auto index(Any index) -> Result<Any> = 0;
  };
}
