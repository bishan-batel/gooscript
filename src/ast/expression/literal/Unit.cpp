//
// Created by bishan_ on 4/26/24.
//

#include "Unit.hpp"

namespace goos::ast::expression {
  Unit::Unit() = default;

  String Unit::to_string() const {
    return "unit";
  }

  Box<Expression> Unit::clone_expr() const { return crab::make_box<Unit>(); }
}
