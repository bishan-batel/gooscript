#include "token.hpp"
#include "box.hpp"

KeywordToken::KeywordToken(const Keyword keyword) : keyword(keyword) {}

Box<Token> KeywordToken::clone() const {
  return box::make<KeywordToken>(keyword);
}
