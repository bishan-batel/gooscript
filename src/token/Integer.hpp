//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include "Token.hpp"

namespace goos::token {
  inline static const Set<widechar> DIGIT_CHARS{
    L'0',
    L'1',
    L'2',
    L'3',
    L'4',
    L'5',
    L'6',
    L'7',
    L'8',
    L'9',
  };

  class Integer final : public Token {
    i64 number;

  public:
    explicit Integer(i64 number);

    [[nodiscard]] i64 get_number() const;

    [[nodiscard]] Box<Token> clone() const override;

    [[nodiscard]] WideString to_string() const override;

    [[nodiscard]] bool operator==(const Token &other) const override;
  };
}
