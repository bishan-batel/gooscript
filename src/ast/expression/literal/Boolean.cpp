//
// Created by bishan_ on 4/26/24.
//

#include "Boolean.hpp"

#include <fmt/format.h>
#include <fmt/xchar.h>

namespace goos::ast::expression {
  bool Boolean::get_state() const { return state; }

  WideString Boolean::to_string() const {
    return fmt::format(L"{}", state);
  }

  Box<Expression> Boolean::clone_expr() const { return crab::make_box<Boolean>(state); }

  bool Boolean::operator==(const Statement &statement) const {
    if (auto other{crab::ref::cast<Boolean>(statement)}) {
      return other.take_unchecked()->state == state;
    }
    return false;
  }
}
