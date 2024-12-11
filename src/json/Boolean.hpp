
//
// Created by bishan_ on 5/8/24.
//

#pragma once
#include "json/Value.hpp"

namespace goos::json {
  class Boolean final : public Value {
    bool state;

  public:
    explicit Boolean(bool state = 0.f);

    auto set(bool s) -> void;

    auto get() const -> bool;

    auto write(std::wostream &os) const -> void override;

    auto clone() const -> Box<Value> override;
  };
}
