//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include <preamble.hpp>

#include "Token.hpp"

namespace goos::token {
  static const Set INVALID_IDENTIFIER_CHARS{
    '[',
    ']',
    '(',
    ')',
    '!',
    '@',
    '#',
    '$',
    '%',
    '^',
    '&',
    '*',
    '(',
    ')',
    '-',
    '+',
    '{',
    '}',
    '<',
    '>',
    ',',
    '.',
    '`',
    '~',
    '/',
    '\\',
    ';',
    ':',
    '\'',
    '"',
    '\t',
    '\n',
    '\r',
    '\0',
    ' ',
  };

  class Identifier final : public Token {
    String identifier;

  public:
    explicit Identifier(String word);

    [[nodiscard]] StringView get_identifier() const;

    [[nodiscard]] Box<Token> clone() const override;

    [[nodiscard]] String to_string() const override;
  };
}
