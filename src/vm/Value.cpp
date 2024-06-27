//
// Created by bishan_ on 6/21/24.
//

#include "Value.hpp"

namespace goos::vm {
  auto Value::operator==(const Value &other) const -> bool {
    return match(
      crab::cases{
        []<typename T>(const T a, const T b) -> bool { return a == b; },
        [](auto, auto) -> bool { return false; }
      },
      other
    );
  }
} // goos
