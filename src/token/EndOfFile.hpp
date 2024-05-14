//
// Created by bishan_ on 5/3/24.
//

#pragma once
#include "Token.hpp"

namespace goos::token {
  class EndOfFile final : public Token {
  public:
    explicit EndOfFile(SourceFile file);

    [[nodiscard]] auto clone() const -> Box<Token> override;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto operator==(const Token &other) const -> bool override;
  };
}
