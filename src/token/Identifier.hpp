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
    Identifier(SourceFile file, Range<> range, WideString identifier);

    [[nodiscard]] auto get_identifier() const -> const WideString&;

    [[nodiscard]] auto clone() const -> Box<Token> override;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto operator==(const Token &other) const -> bool override;
  };
}
