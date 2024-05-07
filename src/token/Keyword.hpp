//
// Created by bishan_ on 4/21/24.
//

#pragma once
#include "Token.hpp"
#include "lexer/Keyword.hpp"

namespace goos::token {
  class Keyword final : public Token {
    lexer::Keyword word;

  public:
    explicit Keyword(lexer::Keyword word);

    [[nodiscard]] auto get_word() const -> lexer::Keyword;

    [[nodiscard]] auto clone() const -> Box<Token> override;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto operator==(const Token &other) const -> bool override;
  };
}
