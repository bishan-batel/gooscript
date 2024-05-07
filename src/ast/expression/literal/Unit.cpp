//
// Created by bishan_ on 4/26/24.
//

#include "Unit.hpp"

namespace goos::ast::expression {
  Unit::Unit() = default;

  WideString Unit::to_string() const {
    return L"unit";
  }

  Box<Expression> Unit::clone_expr() const { return crab::make_box<Unit>(); }

  bool Unit::operator==(const Statement &statement) const {
    return crab::ref::cast<Unit>(statement).is_some();
  }
}
