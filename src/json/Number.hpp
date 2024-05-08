//
// Created by bishan_ on 5/8/24.
//

#pragma once
#include "Value.hpp"

namespace goos::json {
  class Number final : public Value {
    f64 number;

  public:
    explicit Number(f64 number = 0.f);

    auto set(f64 num) -> void;

    auto get() const -> f64;

    auto write(std::wostream &os) const -> void override;

    auto clone() const -> Box<Value> override;
  };
}
