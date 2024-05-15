#pragma once
#include <preamble.hpp>
#include <box.hpp>
#include <fmt/core.h>
#include <source/SourceFile.hpp>

namespace goos::token {
  class Token {
    SourceFile file;
    Range<> range;

  public:
    Token(const Token &) = delete;

    Token(SourceFile file, Range<> range);

    Token(Token &&) = delete;

    virtual ~Token() = default;

    auto operator=(const Token &) -> void = delete;

    auto operator=(Token &&) -> void = delete;

    [[nodiscard]] virtual auto operator==(const Token &other) const -> bool = 0;

    [[nodiscard]] virtual auto clone() const -> Box<Token> = 0;

    [[nodiscard]] auto get_file() const -> const SourceFile&;

    [[nodiscard]] auto get_range() const -> const Range<>&;

    [[nodiscard]] auto get_slice() const -> WideString;

    [[nodiscard]] virtual auto to_string() const -> WideString = 0;

    [[nodiscard]] auto get_line_and_column() const -> std::pair<usize, usize>;

    friend auto operator <<(std::ostream &os, const Token &token) -> std::ostream&;
  };
} // namespace lexer
