//
// Created by bishan_ on 4/19/24.
//
#include "Keyword.hpp"

namespace goos::lexer {
  Option<Keyword> identifier_to_keyword(const std::string_view name) {
    if (KEYWORD_TABLE.contains(name)) {
      return crab::some(KEYWORD_TABLE.at(name));
    }
    return crab::none;
  }
}
