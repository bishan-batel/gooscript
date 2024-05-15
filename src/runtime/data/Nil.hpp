//
// Created by bishan_ on 5/15/24.
//

#pragma once
#include "Value.hpp"

namespace goos::runtime {
  class Nil final : public Value {
  public:
    Nil() = default;

    auto to_string() const -> WideString override;

    auto get_type() const -> meta::VariantType override;
  };
}
