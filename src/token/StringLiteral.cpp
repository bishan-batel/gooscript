//
// Created by bishan_ on 4/25/24.
//

#include "StringLiteral.hpp"

namespace goos::token {
  StringLiteral::StringLiteral(const StringView string) : literal(string) {}

  Box<Token> StringLiteral::clone() const { return crab::make_box<StringLiteral>(*this); }

  String StringLiteral::to_string() const {
    return fmt::format("\"{}\"", literal);
  }

  StringView StringLiteral::get_string() const { return literal; }
}
