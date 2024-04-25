//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include "Token.hpp"

namespace goos::token {
  class Integer final : public Token {
    i64 number;

  public:
    explicit Integer(i64 number);

    [[nodiscard]] i64 get_number() const;

    [[nodiscard]] Box<Token> clone() const override;

    [[nodiscard]] String to_string() const override;
  };
}
