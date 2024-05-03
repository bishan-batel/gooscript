//
// Created by bishan_ on 4/26/24.
//

#include "StringLiteral.hpp"
#include <fmt/core.h>

namespace goos::ast::expression {
  StringLiteral::StringLiteral(String literal) : literal(std::move(literal)) {}

  const String& StringLiteral::get_string() const { return literal; }

  String StringLiteral::to_string() const {
    return fmt::format("\"{}\"", literal);
  }

  Box<Expression> StringLiteral::clone_expr() const {
    return crab::make_box<StringLiteral>(literal);
  }
}
