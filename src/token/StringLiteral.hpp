//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include "Token.hpp"

namespace goos::token {
  class StringLiteral final : public Token {
    String literal;

  public:
    explicit StringLiteral(StringView string);

    [[nodiscard]] Box<Token> clone() const override;

    [[nodiscard]] String to_string() const override;

    [[nodiscard]] StringView get_string() const;
  };
}
