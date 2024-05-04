//
// Created by bishan_ on 4/25/24.
//

#include "StringLiteral.hpp"

namespace goos::token {
  StringLiteral::StringLiteral(const StringView string) : literal(string) {}

  Box<Token> StringLiteral::clone() const { return crab::make_box<StringLiteral>(literal); }

  String StringLiteral::to_string() const {
    return fmt::format("\"{}\"", literal);
  }

  StringView StringLiteral::get_string() const { return literal; }

  bool StringLiteral::operator==(const Token &other) const {
    if (Option<Ref<StringLiteral>> ref = crab::ref::cast<StringLiteral>(other)) {
      return ref.get_unchecked()->literal == literal;
    }
    return false;
  }
}
