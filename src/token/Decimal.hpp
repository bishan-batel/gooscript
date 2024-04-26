//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include "Token.hpp"

namespace goos::token {
  class Decimal final : public Token {
    f64 number;

  public:
    explicit Decimal(f64 num);

    [[nodiscard]] f64 get_number() const;

    [[nodiscard]] Box<Token> clone() const override;

    [[nodiscard]] String to_string() const override;

    [[nodiscard]] bool operator==(const Token &other) const override;
  };
}
