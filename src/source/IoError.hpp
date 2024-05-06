//
// Created by bishan_ on 5/5/24.
//

#pragma once

#include "result.hpp"

namespace goos::io {
  class Error final : public crab::Error {
    i32 error_code;

    Error() = default;

  public:
    explicit Error(i32 error_code);

    [[nodiscard]] String what() const override;

    [[nodiscard]] i32 get_system_error_code() const;
  };
}
