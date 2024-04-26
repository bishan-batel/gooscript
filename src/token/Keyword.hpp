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

    [[nodiscard]] lexer::Keyword get_word() const;

    [[nodiscard]] Box<Token> clone() const override;

    [[nodiscard]] String to_string() const override;

    [[nodiscard]] bool operator==(const Token &other) const override;
  };
}
