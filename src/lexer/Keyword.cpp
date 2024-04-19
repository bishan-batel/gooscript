//
// Created by bishan_ on 4/19/24.
//
#include "Keyword.hpp"

namespace lexer {
  Option<Keyword> identifier_to_keyword(std::string_view view) {
    if (KEYWORD_TABLE.contains(view)) {
      return opt::some(KEYWORD_TABLE.at(view));
    }
    return opt::none;
  }

}
