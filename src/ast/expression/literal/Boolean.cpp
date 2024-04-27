//
// Created by bishan_ on 4/26/24.
//

#include "Boolean.hpp"

#include <fmt/compile.h>

namespace goos::ast::expression {
  bool Boolean::get_state() const { return state; }

  String Boolean::to_string() const {
    return fmt::format(":boolalpha", state);
  }

  Option<meta::VariantType> Boolean::variant_type() const {
    return crab::some(meta::VariantType::BOOLEAN);
  }
}
