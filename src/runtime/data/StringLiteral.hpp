//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include "Value.hpp"

namespace goos::runtime {
  class StringLiteral final : public Value {
    WideString text;

  public:
    explicit StringLiteral(WideString string);

    auto set(WideString v) -> void;

    [[nodiscard]] auto get() const -> const WideString&;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;
  };
}
