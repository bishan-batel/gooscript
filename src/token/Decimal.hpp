//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include "Token.hpp"

namespace goos::token {
  class Decimal final : public Token {
    f64 number;

  public:
    Decimal(SourceFile file, Range<> range, f64 number);

    [[nodiscard]] auto get_number() const -> f64;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto operator==(const Token &other) const -> bool override;
  };
}
