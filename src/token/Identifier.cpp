//
// Created by bishan_ on 4/25/24.
//

#include "Identifier.hpp"
#include <fmt/xchar.h>

namespace goos::token {
  Identifier::Identifier(WideString word) : identifier{std::move(word)} {}

  const WideString& Identifier::get_identifier() const { return identifier; }

  Box<Token> Identifier::clone() const { return crab::make_box<Identifier>(identifier); }

  WideString Identifier::to_string() const {
    return fmt::format(L"{}", identifier);
  }

  bool Identifier::operator==(const Token &other) const {
    if (const Option<Ref<Identifier>> num = crab::ref::cast<Identifier>(other)) {
      return num.get_unchecked()->identifier == identifier;
    }
    return false;
  }
}
