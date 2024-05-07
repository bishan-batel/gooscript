//
// Created by bishan_ on 4/25/24.
//

#include "Identifier.hpp"
#include <fmt/xchar.h>

namespace goos::token {
  Identifier::Identifier(WideString word) : identifier{std::move(word)} {}

  auto Identifier::get_identifier() const -> const WideString& { return identifier; }

  auto Identifier::clone() const -> Box<Token> { return crab::make_box<Identifier>(identifier); }

  auto Identifier::to_string() const -> WideString {
    return fmt::format(L"{}", identifier);
  }

  auto Identifier::operator==(const Token &other) const -> bool {
    if (const Option<Ref<Identifier>> num = crab::ref::cast<Identifier>(other)) {
      return num.get_unchecked()->identifier == identifier;
    }
    return false;
  }
}
