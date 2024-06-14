//
// Created by bishan_ on 5/15/24.
//

#pragma once
#include <memory>

#include "preamble.hpp"
#include "data/Value.hpp"

namespace goos::runtime {
  class Enviornment final {
    Dictionary<WideString, std::shared_ptr<Value>> values{};
    Option<Ref<Enviornment>> parent;
    usize depth{0};

  public:
    explicit Enviornment(Option<Ref<Enviornment>> parent = crab::none);

    [[nodiscard]] auto get_depth() const -> usize;
  };
}
