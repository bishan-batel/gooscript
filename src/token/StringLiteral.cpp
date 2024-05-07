//
// Created by bishan_ on 4/25/24.
//

#include "StringLiteral.hpp"
#include <fmt/xchar.h>

namespace goos::token {
  StringLiteral::StringLiteral(WideString string) : literal(std::move(string)) {}

  Box<Token> StringLiteral::clone() const { return crab::make_box<StringLiteral>(literal); }

  WideString StringLiteral::to_string() const {
    return fmt::format(L"\"{}\"", literal);
  }

  const WideString& StringLiteral::get_string() const { return literal; }

  bool StringLiteral::operator==(const Token &other) const {
    if (Option<Ref<StringLiteral>> ref = crab::ref::cast<StringLiteral>(other)) {
      return ref.get_unchecked()->literal == literal;
    }
    return false;
  }
}
