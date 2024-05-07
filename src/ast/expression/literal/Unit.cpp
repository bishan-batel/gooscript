//
// Created by bishan_ on 4/26/24.
//

#include "Unit.hpp"

namespace goos::ast::expression {
  Unit::Unit() = default;

  auto Unit::to_string() const -> WideString {
    return L"unit";
  }

  auto Unit::clone_expr() const -> Box<Expression> { return crab::make_box<Unit>(); }

  auto Unit::operator==(const Statement &statement) const -> bool {
    return crab::ref::cast<Unit>(statement).is_some();
  }
}
