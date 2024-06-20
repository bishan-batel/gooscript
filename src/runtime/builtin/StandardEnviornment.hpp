//
// Created by bishan_ on 6/19/24.
//

#pragma once
#include "runtime/Environment.hpp"

namespace goos::runtime::builtin {
  auto get_standard_enviornment() -> RcMut<Environment>;
}
