//
// Created by bishan_ on 5/7/24.
//

#pragma once

namespace goos::meta {
  /**
   * @brief States for which a state can change
   */
  enum class Mutability {
    /**
     * @brief Compile-Time constant
     */
    CONSTANT,

    /**
     * @brief Variables that do not change.
     */
    IMMUTABLE,

    /**
     * @brief Variables that can change.
     */
    MUTABLE
  };
} // goos
