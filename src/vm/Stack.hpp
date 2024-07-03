//
// Created by bishan_ on 6/24/24.
//

#pragma once
#include <vector>

#include "Value.hpp"

namespace goos::vm {
  class Stack {
    std::vector<Value> buffer{};
    usize top{0};

  public:
    Stack();

    [[nodiscard]] auto peek(usize offset = 0) -> Result<Value>;

    [[nodiscard]] auto peek(usize offset = 0) const -> Result<Value>;

    auto push(Value value) -> Result<unit>;

    auto pop() -> Result<Value>;

    [[nodiscard]] auto size() const -> usize;

    [[nodiscard]] auto empty() const -> usize;
  };
} // goos
