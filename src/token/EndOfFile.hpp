//
// Created by bishan_ on 5/3/24.
//

#pragma once
#include "Token.hpp"

namespace goos::token {
  class EndOfFile final : public Token {
  public:
    explicit EndOfFile();

    [[nodiscard]] Box<Token> clone() const override;

    [[nodiscard]] String to_string() const override;

    [[nodiscard]] bool operator==(const Token &other) const override;
  };
}
