//
// Created by bishan_ on 4/26/24.
//

#include "Boolean.hpp"

#include <fmt/compile.h>

namespace goos::ast::expression {
  bool Boolean::get_state() const { return state; }

  String Boolean::to_string() const {
    return fmt::format("{}", state);
  }

  Box<Expression> Boolean::clone_expr() const { return crab::make_box<Boolean>(state); }
}
