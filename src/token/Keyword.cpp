//
// Created by bishan_ on 4/21/24.
//

#include "Keyword.hpp"

namespace goos::token {
  Keyword::Keyword(const lexer::Keyword word) : word(word) {}

  Box<Token> Keyword::clone() const {
    throw std::logic_error("Not implemented");
  }
}
