//
// Created by bishan_ on 6/26/24.
//

#pragma once
#include "Error.hpp"
#include <fmt/format.h>

namespace goos::vm::err {
  class CastError final : public Error {
    StringView from{}, to{};

  public:
    CastError(const StringView from, const StringView to)
      : from{from},
        to{to} {}

    [[nodiscard]] auto what() const -> String override {
      return fmt::format("Invalid cast from {} to {}", from, to);
    }
  };
}
