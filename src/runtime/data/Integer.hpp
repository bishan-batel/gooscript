//
// Created by bishan_ on 5/15/24.
//

#pragma once
#include "Value.hpp"

namespace goos::runtime {
  class Integer final : public Value {
    i64 value;

  public:
    using Contained = i64;

    explicit Integer(i64 value = 0);

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    auto set(i64 v) -> void;

    [[nodiscard]] auto get() const -> i64;
  };
}
