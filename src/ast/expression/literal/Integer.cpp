//
// Created by bishan_ on 4/26/24.
//

#include "Integer.hpp"

#include <fmt/format.h>
#include <fmt/xchar.h>

namespace goos::ast::expression {
  Integer::Integer(const i64 number): number{number} {}

  WideString Integer::to_string() const {
    return fmt::format(L"{}", number);
  }

  Box<Expression> Integer::clone_expr() const { return crab::make_box<Integer>(number); }

  bool Integer::operator==(const Statement &statement) const {
    if (auto other = crab::ref::cast<Integer>(statement)) {
      return other.take_unchecked()->number == number;
    }
    return false;
  }
}
