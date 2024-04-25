#pragma once
#include <preamble.hpp>
#include <box.hpp>

namespace goos::token {
  class Token {
    StringView slice;

  public:
    Token(const Token &) = default;

    Token() = default;

    Token(Token &&) = delete;

    virtual ~Token() = default;

    void operator=(const Token &) = delete;

    void operator=(Token &&) = delete;

    [[nodiscard]] virtual Box<Token> clone() const = 0;

    [[nodiscard]] StringView get_slice() const;
  };
} // namespace lexer
