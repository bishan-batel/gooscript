//
// Created by bishan_ on 5/15/24.
//

#pragma once
#include "IValue.hpp"

namespace goos::runtime {
  class Nil final : public IValue {
  public:
    Nil() = default;

    auto to_string() const -> WideString override;

    auto get_type() const -> meta::VariantType override;
  };
}
