//
// Created by bishan_ on 5/15/24.
//

#pragma once
#include <memory>

#include "preamble.hpp"
#include "data/Value.hpp"
#include "meta/Identifier.hpp"

namespace goos::runtime {
  class Environment final {
    Dictionary<meta::Identifier, std::shared_ptr<Value>> values{};
    Option<Ref<Environment>> parent;
    usize depth{0};

  public:
    explicit Environment(Option<Ref<Environment>> parent = crab::none);

    [[nodiscard]] auto get_depth() const -> usize;
  };
}
