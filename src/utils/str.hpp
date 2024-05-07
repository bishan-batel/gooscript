//
// Created by bishan_ on 5/7/24.
//

#pragma once

#include "preamble.hpp"

namespace goos::str {
  [[nodiscard]] auto convert(WideStringView string) -> String;

  [[nodiscard]] auto convert(StringView string) -> WideString;
}
