//
// Created by bishan_ on 4/25/24.
//

#include "Identifier.hpp"

namespace goos::token {
  Identifier::Identifier(String word) : identifier{std::move(word)} {}

  StringView Identifier::get_identifier() const { return identifier; }

  Box<Token> Identifier::clone() const { return crab::make_box<Identifier>(*this); }

  String Identifier::to_string() const {
    return fmt::format("{}", identifier);
  }
}
