//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include <preamble.hpp>

#include "Token.hpp"

namespace goos::token {
  inline static const Set<widechar> INVALID_IDENTIFIER_CHARS{
    L'[',
    L']',
    L'(',
    L')',
    L'!',
    L'@',
    L'#',
    L'$',
    L'%',
    L'^',
    L'&',
    L'*',
    L'(',
    L')',
    L'-',
    L'+',
    L'{',
    L'}',
    L'<',
    L'>',
    L',',
    L'.',
    L'`',
    L'~',
    L'/',
    L'\\',
    L';',
    L':',
    L'\'',
    L'"',
    L'\t',
    L'\n',
    L'\r',
    L'\0',
    L' ',
  };

  class Identifier final : public Token {
    WideString identifier;

  public:
    explicit Identifier(WideString word);

    [[nodiscard]] const WideString& get_identifier() const;

    [[nodiscard]] Box<Token> clone() const override;

    [[nodiscard]] WideString to_string() const override;

    [[nodiscard]] bool operator==(const Token &other) const override;
  };
}
