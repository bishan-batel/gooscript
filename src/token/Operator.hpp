//
// Created by bishan_ on 4/21/24.
//

#pragma once
#include <lexer/Operator.hpp>
#include "Token.hpp"

namespace goos::token {
  class Operator final : public Token {
    lexer::Operator op;

  public:
    explicit Operator(lexer::Operator op);

    [[nodiscard]] auto get_op() const -> lexer::Operator;

    [[nodiscard]] auto clone() const -> Box<Token> override;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto operator==(const Token &other) const -> bool override;
  };
}
