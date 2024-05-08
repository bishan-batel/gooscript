#pragma once
#include <preamble.hpp>
#include <box.hpp>
#include <fmt/core.h>

namespace goos::token {
  class Token {
    WideStringView slice;

  public:
    Token(const Token &) = delete;

    Token() = default;

    Token(Token &&) = delete;

    virtual ~Token() = default;

    auto operator=(const Token &) -> void = delete;

    auto operator=(Token &&) -> void = delete;

    [[nodiscard]] virtual auto operator==(const Token &other) const -> bool = 0;

    [[nodiscard]] virtual auto clone() const -> Box<Token> = 0;

    [[nodiscard]] auto get_slice() const -> WideStringView;

    [[nodiscard]] virtual auto to_string() const -> WideString = 0;

    friend auto operator <<(std::ostream &os, const Token &token) -> std::ostream&;
  };
} // namespace lexer
