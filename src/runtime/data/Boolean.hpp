//
// Created by bishan_ on 5/16/24.
//

#pragma once
#include "IValue.hpp"

namespace goos::runtime {
  class Boolean final : public IValue {
    bool state;

  public:
    explicit Boolean(bool state = false);

    // auto set(bool v) -> void;

    [[nodiscard]] auto get() const -> bool;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    auto is_truthy() const -> bool override;

    [[nodiscard]] auto base_hash() const -> usize override;

    [[nodiscard]] auto clone() const -> Any override;
  };
}
