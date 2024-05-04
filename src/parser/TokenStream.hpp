//
// Created by bishan_ on 5/3/24.
//

#pragma once
#include <lexer/Keyword.hpp>
#include <lexer/Operator.hpp>

#include "Error.hpp"
#include "lexer/Lexer.hpp"
#include "token/EndOfFile.hpp"

namespace goos::parser {
  template<typename T>
  using Result = Result<T, err::Error>;

  class TokenStream {
    lexer::TokenList list;
    usize position{0};
    token::EndOfFile eof;

  public:
    explicit TokenStream(lexer::TokenList list);

    [[nodiscard]] const token::Token& curr() const;

    [[nodiscard]] const token::Token& next();

    template<typename T> requires std::is_base_of_v<token::Token, T>
    [[nodiscard]] bool is_curr() const {
      return crab::ref::cast<T>(curr()).is_some();
    }

    [[nodiscard]] bool is_curr(const token::Token &token) const;

    [[nodiscard]] bool is_curr(const lexer::Operator &token) const;

    [[nodiscard]] bool is_curr(const lexer::Keyword &token) const;

    [[nodiscard]] bool is_eof() const;
  };
}
