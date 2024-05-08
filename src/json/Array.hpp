//
// Created by bishan_ on 5/8/24.
//

#pragma once
#include "Value.hpp"

namespace goos::json {
  class Array final : public Value {
    Vec<Box<Value>> values{};

  public:
    Array();

    auto push(Box<Value> value) -> void;

    auto length() const -> usize;

    auto write(std::wostream &os) const -> void override;

    auto clone() const -> Box<Value> override;
  };
}
