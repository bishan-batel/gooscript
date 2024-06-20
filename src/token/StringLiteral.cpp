//
// Created by bishan_ on 4/25/24.
//

#include "StringLiteral.hpp"
#include <fmt/xchar.h>

namespace goos::token {
  StringLiteral::StringLiteral(SourceFile file, const Range<> range, Rc<WideString> literal)
    : Token{std::move(file), range},
      literal{std::move(literal)} {}

  auto StringLiteral::to_string() const -> WideString {
    return fmt::format(L"\"{}\"", *literal);
  }

  auto StringLiteral::get_string() const -> Rc<WideString> { return literal; }

  auto StringLiteral::operator==(const Token &other) const -> bool {
    if (Option<Ref<StringLiteral>> ref = crab::ref::cast<StringLiteral>(other)) {
      return ref.get_unchecked()->literal == literal;
    }
    return false;
  }
}
