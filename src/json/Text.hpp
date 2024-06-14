//
// Created by bishan_ on 5/8/24.
//

#pragma once
#include "Value.hpp"

namespace goos::json {
  class Text final : public Value {
    WideString text;

  public:
    explicit Text(WideString text);

    auto set_text(WideString value) -> void;

    auto write(std::wostream &os) const -> void override;

    [[nodiscard]] auto get_text() const -> const WideString&;

    [[nodiscard]] auto clone() const -> Box<Value> override;
  };
}
