//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include <preamble.hpp>

#include "Token.hpp"

using schar = i8;
using uchar = u8;

namespace goos::token {
  static const Set<uchar> INVALID_IDENTIFIER_CHARS{
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

    [[nodiscard]] const String& get_identifier() const;

    [[nodiscard]] Box<Token> clone() const override;

    [[nodiscard]] String to_string() const override;

    [[nodiscard]] bool operator==(const Token &other) const override;
  };
}
