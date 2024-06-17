//
// Created by bishan_ on 5/15/24.
//

#pragma once
#include "IValue.hpp"

namespace goos::runtime {
  class Nil final : public IValue {
  public:
    static constexpr meta::VariantType TYPE{meta::VariantType::NIL};

    static auto value() -> RcMut<Nil>;

    Nil() = default;

    explicit Nil(std::nullptr_t);

    auto to_string() const -> WideString override;

    auto get_type() const -> meta::VariantType override;

    auto base_hash() const -> usize override;

    [[nodiscard]] auto clone() const -> Any override;
  };
}
