//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include "Token.hpp"

namespace goos::token {
  class StringLiteral final : public Token {
    WideString literal;

  public:
    explicit StringLiteral(WideString string);

    [[nodiscard]] auto clone() const -> Box<Token> override;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_string() const -> const WideString&;

    [[nodiscard]] auto operator==(const Token &other) const -> bool override;
  };
}
