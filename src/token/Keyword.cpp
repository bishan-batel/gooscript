//
// Created by bishan_ on 4/21/24.
//

#include "Keyword.hpp"

namespace goos::token {
  Keyword::Keyword(const lexer::Keyword word) : word(word) {}

  lexer::Keyword Keyword::get_word() const { return word; }

  Box<Token> Keyword::clone() const {
    return crab::make_box<Keyword>(*this);
  }
}
