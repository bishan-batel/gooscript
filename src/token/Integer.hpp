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

    [[nodiscard]] auto get_number() const -> i64;

    [[nodiscard]] auto clone() const -> Box<Token> override;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto operator==(const Token &other) const -> bool override;
  };
}
