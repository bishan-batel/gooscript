//
// Created by bishan_ on 4/26/24.
//

#include "StringLiteral.hpp"
#include <fmt/format.h>
#include <fmt/xchar.h>

namespace goos::ast::expression {
  StringLiteral::StringLiteral(WideString literal) : literal(std::move(literal)) {}

  const WideString& StringLiteral::get_string() const { return literal; }

  WideString StringLiteral::to_string() const {
    return fmt::format(L"\"{}\"", literal);
  }

  Box<Expression> StringLiteral::clone_expr() const {
    return crab::make_box<StringLiteral>(literal);
  }

  bool StringLiteral::operator==(const Statement &statement) const {
    if (auto other = crab::ref::cast<StringLiteral>(statement)) {
      return other.take_unchecked()->literal == literal;
    }
    return false;
  }
}
