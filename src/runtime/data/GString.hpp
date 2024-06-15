//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include "IValue.hpp"

namespace goos::runtime {
  class GString final : public IValue {
    WideString text;

  public:
    using Contained = WideString;

    explicit GString(WideString string = L"");

    auto set(WideString v) -> void;

    [[nodiscard]] auto get() const -> const WideString&;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;
  };
}
