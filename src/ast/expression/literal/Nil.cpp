//
// Created by bishan_ on 4/27/24.
//

#include "Nil.hpp"

namespace goos::ast::expression {
  WideString Nil::to_string() const {
    return L"nil";
  }

  Box<Expression> Nil::clone_expr() const { return crab::make_box<Nil>(); }

  bool Nil::operator==(const Statement &statement) const {
    return crab::ref::cast<Nil>(statement).is_some();
  }
} // goos
