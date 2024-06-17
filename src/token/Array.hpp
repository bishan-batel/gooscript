//
// Created by bishan_ on 6/16/24.
//

#pragma once
#include "meta/VariantType.hpp"
#include "runtime/data/IValue.hpp"

namespace goos::runtime {
  class Array final : public IValue {
    Vec<Any> values{};

  public:
    auto push(Any value) -> void;

    [[nodiscard]] auto clone() const -> Any override;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    [[nodiscard]] auto base_hash() const -> utils::hash_code override;

    [[nodiscard]] auto get_values()  const -> Vec<Any>;

    [[nodiscard]] auto length() const -> usize;

    constexpr static meta::VariantType TYPE = meta::VariantType::ARRAY;
  };
} // goos
