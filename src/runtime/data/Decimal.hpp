//
// Created by bishan_ on 5/16/24.
//

#pragma once
#include "interfaces/INumeric.hpp"

namespace goos::runtime {
  class Decimal final : public INumeric {
    f64 value;

  public:
    static constexpr meta::VariantType TYPE{meta::VariantType::DECIMAL};

    using Contained = f64;

    explicit Decimal(f64 value = 0.f);

    [[nodiscard]] auto get() const -> f64;

    // auto set(f64 v) -> void;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    [[nodiscard]] auto get_number() const -> f64 override;

    [[nodiscard]] auto is_integral() const -> bool override;

    auto base_hash() const -> usize override;

    [[nodiscard]] auto clone() const -> Any override;

    [[nodiscard]] auto to_json() const -> Box<json::Value> override;
  };
}
