//
// Created by bishan_ on 4/27/24.
//

#include "Nil.hpp"

namespace goos::ast::expression {
  auto Nil::to_string() const -> WideString {
    return L"nil";
  }

  auto Nil::clone_expr() const -> Box<Expression> { return crab::make_box<Nil>(); }

  auto Nil::operator==(const Statement &statement) const -> bool {
    return crab::ref::cast<Nil>(statement).is_some();
  }
} // goos
