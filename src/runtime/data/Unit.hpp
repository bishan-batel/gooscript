//
// Created by bishan_ on 5/16/24.
//

#pragma once
#include "IValue.hpp"

namespace goos::runtime {
  class Unit final : public IValue {
  public:
    Unit();

    auto to_string() const -> WideString override;

    auto get_type() const -> meta::VariantType override;

    auto hash() const -> usize override;
  };
}
