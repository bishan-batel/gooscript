//
// Created by bishan_ on 5/15/24.
//

#include "Enviornment.hpp"

namespace goos::runtime {
  Enviornment::Enviornment(const Option<Ref<Enviornment>> parent)
    : parent{parent} {
    if (auto p = parent) {
      depth = p.get_unchecked()->depth + 1;
    }
  }

  auto Enviornment::get_depth() const -> usize {
    return depth;
  }
}
