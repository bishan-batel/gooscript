//
// Created by bishan_ on 4/21/24.
//

#include "Keyword.hpp"

namespace goos::token {

  Keyword::Keyword(SourceFile file, const Range<> range, const lexer::Keyword word)
    : Token{std::move(file), range},
      word{word} {}

  auto Keyword::get_word() const -> lexer::Keyword { return word; }

  auto Keyword::clone() const -> Box<Token> {
    return crab::make_box<Keyword>(get_file(), get_range(), word);
  }

  auto Keyword::to_string() const -> WideString {
    return WideString{lexer::KEYWORD_TO_STR_MAP.at(word)};
  }

  auto Keyword::operator==(const Token &other) const -> bool {
    if (const Option<Ref<Keyword>> keyword = crab::ref::cast<Keyword>(other)) {
      return keyword.get_unchecked()->word == word;
    }
    return false;
  }
}
