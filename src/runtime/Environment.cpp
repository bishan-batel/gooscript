//
// Created by bishan_ on 5/15/24.
//

#include "Environment.hpp"

namespace goos::runtime {
  Environment::Environment(const Option<Ref<Environment>> parent)
    : parent{parent} {
    if (auto p = parent) {
      depth = p.get_unchecked()->depth + 1;
    }
  }

  auto Environment::get_depth() const -> usize {
    return depth;
  }
}
