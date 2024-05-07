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

    [[nodiscard]] Box<Token> clone() const override;

    [[nodiscard]] WideString to_string() const override;

    [[nodiscard]] const WideString& get_string() const;

    [[nodiscard]] bool operator==(const Token &other) const override;
  };
}
