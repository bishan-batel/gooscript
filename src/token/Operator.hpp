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

    [[nodiscard]] lexer::Operator get_op() const;

    [[nodiscard]] Box<Token> clone() const override;

    [[nodiscard]] WideString to_string() const override;

    [[nodiscard]] bool operator==(const Token &other) const override;
  };
}
