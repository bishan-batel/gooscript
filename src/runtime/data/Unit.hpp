//
// Created by bishan_ on 5/16/24.
//

#pragma once
#include "IValue.hpp"

namespace goos::runtime {
  class Unit final : public IValue {
  public:
    static auto value() -> RcMut<Unit>;

    Unit();

    auto to_string() const -> WideString override;

    auto get_type() const -> meta::VariantType override;

    auto base_hash() const -> usize override;

    [[nodiscard]] auto clone() const -> Any override;
  };
}
