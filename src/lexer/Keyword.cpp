//
// Created by bishan_ on 4/19/24.
//
#include "Keyword.hpp"

namespace goos::lexer {
  auto identifier_to_keyword(const WideStringView name) -> Option<Keyword> {
    if (STR_TO_KEYWORD_TABLE.contains(name)) {
      return crab::some(STR_TO_KEYWORD_TABLE.at(name));
    }
    return crab::none;
  }
}
