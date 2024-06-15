//
// Created by bishan_ on 5/16/24.
//

#pragma once
#include "Value.hpp"

namespace goos::runtime {
  class Boolean final : public Value {
    bool state;

  public:
    explicit Boolean(bool state);

    auto set(bool v) -> void;

    [[nodiscard]] auto get() const -> bool;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    auto is_truthy() const -> bool override;
  };
}
