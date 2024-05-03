//
// Created by bishan_ on 4/26/24.
//

#include "Integer.hpp"

#include <fmt/compile.h>

namespace goos::ast::expression {
  Integer::Integer(const i64 number): number{number} {}

  String Integer::to_string() const {
    return fmt::format("{}", number);
  }

  Box<Expression> Integer::clone_expr() const { return crab::make_box<Integer>(number); }
}
