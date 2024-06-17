//
// Created by bishan_ on 5/16/24.
//

#pragma once
#include "IValue.hpp"

namespace goos::runtime {
  class Unit final : public IValue {
  public:
    static constexpr meta::VariantType TYPE{meta::VariantType::UNIT};

    static auto value() -> RcMut<Unit>;

    Unit();

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    [[nodiscard]] auto base_hash() const -> usize override;

    [[nodiscard]] auto clone() const -> Any override;
  };
}
