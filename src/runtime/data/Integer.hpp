//
// Created by bishan_ on 5/15/24.
//

#pragma once
#include "INumeric.hpp"
#include "IValue.hpp"

namespace goos::runtime {
  class Integer final : public INumeric {
    i64 value;

  public:
    using Contained = i64;

    static constexpr meta::VariantType TYPE{meta::VariantType::INTEGER};

    explicit Integer(i64 value = 0);

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    auto set(i64 v) -> void;

    [[nodiscard]] auto get() const -> i64;

    [[nodiscard]] auto get_number() const -> f64 override;

    [[nodiscard]] auto is_integral() const -> bool override;

    [[nodiscard]] auto base_hash() const -> usize override;

    [[nodiscard]] auto clone() const -> Any override;
  };
}
