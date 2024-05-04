#pragma once
#include <preamble.hpp>
#include <box.hpp>
#include <fmt/core.h>

namespace goos::token {
  class Token {
    StringView slice;

  public:
    Token(const Token &) = delete;

    Token() = default;

    Token(Token &&) = delete;

    virtual ~Token() = default;

    void operator=(const Token &) = delete;

    void operator=(Token &&) = delete;

    [[nodiscard]] virtual bool operator==(const Token &other) const = 0;

    [[nodiscard]] virtual Box<Token> clone() const = 0;

    [[nodiscard]] StringView get_slice() const;

    [[nodiscard]] virtual String to_string() const = 0;
  };
} // namespace lexer
