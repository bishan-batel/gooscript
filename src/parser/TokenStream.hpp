//
// Created by bishan_ on 5/3/24.
//

#pragma once
#include "lexer/Lexer.hpp"

namespace goos::parser {
  class TokenStream {
    lexer::TokenList list;

  public:
    explicit TokenStream(lexer::TokenList list);

    [[nodiscard]] bool is_eof() const;
  };
}
