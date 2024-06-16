//
// Created by bishan_ on 6/15/24.
//

#include "hash.hpp"

#include <stdexcept>

namespace goos::utils {
  auto combine_hash(const usize seed, const usize next) -> usize {
    return next + 0x9e3779b9 + (seed << 6) + (seed >> 2) ^ seed;
  }
}
