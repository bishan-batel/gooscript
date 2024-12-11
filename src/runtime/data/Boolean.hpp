//
// Created by bishan_ on 5/16/24.
//

#pragma once
#include "interfaces/IValue.hpp"

namespace goos::runtime {
  class Boolean final : public IValue {
    bool state;

  public:
    static constexpr meta::VariantType TYPE{meta::VariantType::BOOLEAN};

    explicit Boolean(bool state = false);

    // auto set(bool v) -> void;

    [[nodiscard]] auto get() const -> bool;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    [[nodiscard]] auto is_truthy() const -> bool override;

    [[nodiscard]] auto base_hash() const -> usize override;

    [[nodiscard]] auto clone() const -> Any override;

    [[nodiscard]] auto to_json() const -> Box<json::Value> override;
  };
}
