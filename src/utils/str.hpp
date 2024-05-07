//
// Created by bishan_ on 5/7/24.
//

#pragma once
#include <stdexcept>

#include "preamble.hpp"

namespace goos::str {
  [[nodiscard]] String convert(WideStringView string);

  [[nodiscard]] WideString convert(StringView string);
}
