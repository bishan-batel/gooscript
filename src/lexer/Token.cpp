#include "Token.hpp"
#include "box.hpp"

namespace lexer {

KeywordToken::KeywordToken(const Keyword keyword_) : keyword(keyword_) {}

Box<Token> KeywordToken::clone() const {
  return box::make<KeywordToken>(keyword);
}

} // namespace lexer
