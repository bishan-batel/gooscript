//
// Created by bishan_ on 4/23/24.
//

#pragma once
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

    [[nodiscard]] auto what() const -> String override;

    [[nodiscard]] auto get_type() const -> Type;
  };
}

namespace goos::lexer {
  template<typename T>
  using Result = Result<T, Error>;

  using TokenList = Vec<Rc<token::Token>>;

  static const Set<widechar> WHITESPACE_CHARS{L' ', L'\t', L'\n', L'\r'};

  class Lexer {
    using Token = token::Token;
    usize position = 0;
    SourceFile file;
    TokenList tokens{};

    explicit Lexer(SourceFile content);

    [[nodiscard]] auto error(Error::Type, usize begin) const -> Error;

    auto push(Rc<Token> token) -> void;

    template<typename T, typename... Args> requires std::is_constructible_v<T, Args...>
    auto emplace(Args... args) -> void {
      push(crab::make_rc<T>(args...));
    }

    [[nodiscard]] auto curr() const -> widechar;

    [[nodiscard]] auto is_curr(widechar c) const -> bool;

    auto next(usize n = 1) -> widechar;

    [[nodiscard]] auto is_eof() const -> bool;

  public:
    static auto tokenize(SourceFile content) -> Result<TokenList>;

  private:
    [[nodiscard]] auto whitespace() -> Result<bool>;

    [[nodiscard]] auto comment() -> Result<bool>;

    [[nodiscard]] auto number_literal() -> Result<bool>;

    [[nodiscard]] auto string_literal() -> Result<bool>;

    [[nodiscard]] auto operator_tok() -> Result<bool>;

    [[nodiscard]] auto identifier() -> Result<bool>;
  };
}
