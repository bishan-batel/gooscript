//
// Created by bishan_ on 5/3/24.
//

#include "TokenStream.hpp"

#include <token/Keyword.hpp>
#include <token/Operator.hpp>

#include "token/EndOfFile.hpp"

namespace goos::parser {
  TokenStream::TokenStream(lexer::TokenList list) : list{std::move(list)} {}

  const token::Token& TokenStream::curr() const {
    if (is_eof()) return eof;
    return list[position];
  }

  const token::Token& TokenStream::next() {
    position++;
    return curr();
  }

  bool TokenStream::is_curr(const token::Token &token) const {
    return curr() == token;
  }

  bool TokenStream::is_curr(const lexer::Operator &token) const {
    return token::Operator{token} == curr();
  }

  bool TokenStream::is_curr(const lexer::Keyword &token) const {
    return token::Keyword{token} == curr();
  }

  bool TokenStream::is_eof() const {
    return position >= list.size();
  }
}
