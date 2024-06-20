//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include "Token.hpp"

namespace goos::token {
  class StringLiteral final : public Token {
    Rc<WideString> literal;

  public:
    StringLiteral(SourceFile file, Range<> range, Rc<WideString> literal);

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_string() const -> Rc<WideString>;

    [[nodiscard]] auto operator==(const Token &other) const -> bool override;
  };
}
