//
// Created by bishan_ on 4/25/24.
//

#include "Identifier.hpp"
#include <fmt/xchar.h>

namespace goos::token {
  Identifier::Identifier(SourceFile file, const Range<> range, WideString identifier)
    : Token{std::move(file), range}, identifier{meta::Identifier::from(std::move(identifier))} {}

  auto Identifier::get_identifier() const -> const meta::Identifier& { return identifier; }

  auto Identifier::to_string() const -> WideString {
    return identifier.get_string();
  }

  auto Identifier::operator==(const Token &other) const -> bool {
    if (const Option<Ref<Identifier>> num = crab::ref::cast<Identifier>(other)) {
      return num.get_unchecked()->identifier == identifier;
    }
    return false;
  }
}
