//
// Created by bishan_ on 5/3/24.
//

#include "TokenStream.hpp"

namespace goos::parser {
  TokenStream::TokenStream(lexer::TokenList list) : list{std::move(list)} {}
}
