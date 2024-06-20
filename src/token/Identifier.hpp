//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include <preamble.hpp>

#include "Token.hpp"
#include "meta/Identifier.hpp"

namespace goos::token {
  inline static const Set<widechar> INVALID_IDENTIFIER_CHARS = [] {
    constexpr WideStringView INVALID{
      L"[]()!@#%^|&*()-+={}<>,.`~/\\;:\'\" \t\n\r"
    };

    std::unordered_set<widechar> chars{'\0'};

    for (widechar c: INVALID) {
      chars.insert(c);
    }

    return chars;
  }();

  class Identifier final : public Token {
    meta::Identifier identifier;

  public:
    Identifier(SourceFile file, Range<> range, WideString identifier);

    [[nodiscard]] auto get_identifier() const -> const meta::Identifier&;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto operator==(const Token &other) const -> bool override;
  };
}
