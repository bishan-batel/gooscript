//
// Created by bishan_ on 6/15/24.
//

#pragma once
#include <preamble.hpp>
#include <utility>

namespace goos::utils {
  template<typename T>
  concept is_hashable = requires(T a) {
    { std::hash<T>{}(a) } -> std::convertible_to<usize>;
  };

  /**
   * @brief Hashes multiple hashes together in a pseudo-random way.
   */
  // https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
  auto combine_hash(usize seed, usize next) -> usize;
}
