//
// Created by bishan_ on 6/15/24.
//

#pragma once
#include <preamble.hpp>
#include <stdexcept>
#include <rc.hpp>

// TODO move this to the crab repository

namespace goos::utils {
  /**
   * @brief Type alias for when dealing with hash values specifically;
   */
  using hash_code = usize;

  /**
   * @brief Any type that can be converted to a hash_code (usize)
   */
  template<typename T>
  concept into_hash_code = std::convertible_to<T, hash_code>;

  template<typename T>
  concept strict_hashable = requires(const T a) {
    { std::hash<T>{}(a) } -> into_hash_code;
  };

  template<typename T>
  concept hashable = strict_hashable<T> or requires(const T a) {
    hashable(*a);
  };

  template<hashable T>
  auto hash(const T &value) -> hash_code;

  /**
   * @brief Hashes together multiple hash codes.
   * https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
   */
  auto hash_code_mix(hash_code seed, hash_code next) -> hash_code;

  /**
   * @brief Hashes together multiple hash codes.
   * https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
   */
  template<into_hash_code First, into_hash_code... Args>
  auto hash_code_mix(First first, Args... rest) -> hash_code;

  ///
  ///@brief Applies goos::utils::hash(x) to each item and mixes the hash codes (with @refitem goos::utils::hash_code_mix ),
  ///the order is dependent on the arguments given.
  template<hashable FirstItem, hashable... Items>
  auto hash_together(FirstItem first, Items... items) -> hash_code;

  ///
  ///@brief Applies goos::utils::hash(x) to each item and mixes the hash codes (with @refitem goos::utils::hash_code_mix ),
  ///the order is dependent on the arguments given.
  template<hashable FirstItem>
  auto hash_together(FirstItem first) -> hash_code;

  // Template Impl

  template<hashable T>
  auto hash(const T &value) -> hash_code {
    return static_cast<hash_code>(std::hash<T>{}(value));
  }

  template<hashable T>
  auto hash(const Rc<T> &value) -> hash_code {
    return utils::hash(*value);
  }

  template<hashable T>
  auto hash(const RcMut<T> &value) -> hash_code {
    return utils::hash(*value);
  }

  template<into_hash_code First, into_hash_code ... Args>
  auto hash_code_mix(First first, Args... rest) -> hash_code {
    return utils::hash_code_mix(first, utils::hash_code_mix(rest...));
  }

  template<hashable FirstItem, hashable ... Items>
  auto hash_together(FirstItem first, Items... items) -> hash_code {
    return utils::hash_code_mix(utils::hash(first), utils::hash_together(items...));
  }

  template<hashable FirstItem>
  auto hash_together(FirstItem first) -> hash_code {
    return utils::hash(first);
  }
}
