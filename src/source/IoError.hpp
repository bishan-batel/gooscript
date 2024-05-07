//
// Created by bishan_ on 5/5/24.
//

#pragma once

#include "result.hpp"

namespace goos::io {
  class Error final : public crab::Error {
    i32 error_code{};

    Error() = default;

  public:
    explicit Error(i32 error_code);

    [[nodiscard]] auto what() const -> String override;

    [[nodiscard]] auto get_system_error_code() const -> i32;
  };
}
