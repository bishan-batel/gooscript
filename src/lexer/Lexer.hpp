//
// Created by bishan_ on 4/23/24.
//

#pragma once
#include <rc.hpp>
#include <box.hpp>
#include <token/Token.hpp>

#include <fmt/format.h>
#include "range.hpp"
#include "result.hpp"
#include "source/SourceFile.hpp"

namespace goos::lexer {
  class Error final : public crab::Error {
  public:
    enum class Type {
      UNTERMINATED_STRING,
      INVALID_ESCAPED_STRING,
      UNKNOWN_CHARACTER
    };

    static auto to_string(Type type) -> StringView;

  private:
    SourceFile section;
    Range<> slice;
    Type type;

  public:
    Error(Type type, SourceFile section, Range<> range);

    [[nodiscard]] String what() const override;

    [[nodiscard]] auto get_type() const -> Type;
  };
}

namespace goos::lexer {
  template<typename T>
  using Result = Result<T, Error>;

  using TokenList = Vec<Box<token::Token>>;

  static const Set<widechar> WHITESPACE_CHARS{L' ', L'\t', L'\n', L'\r'};

  class Lexer {
    using Token = token::Token;
    usize position = 0;
    SourceFile content;
    TokenList tokens{};

    explicit Lexer(SourceFile content);

    [[nodiscard]] Error error(Error::Type, usize begin) const;

    void push(Box<Token> token);

    template<typename T, typename... Args> requires std::is_constructible_v<T, Args...>
    void emplace(Args... args) {
      push(crab::make_box<T>(args...));
    }

    [[nodiscard]] widechar curr() const;

    [[nodiscard]] bool is_curr(widechar c) const;

    widechar next(usize n = 1);

    [[nodiscard]] bool is_eof() const;

  public:
    static Result<TokenList> tokenize(SourceFile content);

  private:
    [[nodiscard]] Result<bool> whitespace();

    [[nodiscard]] Result<bool> number_literal();

    [[nodiscard]] Result<bool> string_literal();

    [[nodiscard]] Result<bool> operator_tok();

    [[nodiscard]] Result<bool> identifier();
  };
}
