//
// Created by bishan_ on 4/27/24.
//

#include "Nil.hpp"

namespace goos::ast::expression {
  String Nil::to_string() const {
    return "nil";
  }

  Box<Expression> Nil::clone_expr() const { return crab::make_box<Nil>(); }
} // goos
