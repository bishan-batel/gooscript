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
    Rc<String> section;
    Range<> slice;
    Type type;

    String msg;

  public:
    Error(Type type, const Rc<String> &section, Range<> range);

    [[nodiscard]] auto what() const -> StringView override;

    [[nodiscard]] auto get_type() const -> Type;
  };
}

namespace goos::lexer {
  template<typename T>
  using Result = Result<T, Error>;

  using TokenList = Vec<Box<token::Token>>;

  static const Set<char> WHITESPACE_CHARS{' ', '\t', '\n', '\r'};

  class Lexer {
    using Token = token::Token;
    usize position = 0;
    Rc<String> content;
    TokenList tokens{};

    explicit Lexer(const Rc<String> &content);

    [[nodiscard]] Error error(Error::Type, usize begin) const;

    void push(Box<Token> token);

    template<typename T, typename... Args> requires std::is_constructible_v<T, Args...>
    void emplace(Args... args) {
      push(crab::make_box<T>(args...));
    }

    [[nodiscard]] char curr() const;

    [[nodiscard]] bool is_curr(char c) const;

    char next(usize n = 1);

    [[nodiscard]] bool is_eof() const;

    [[nodiscard]] StringView substr(Range<> range) const;

  public:
    static Result<TokenList> tokenize(const Rc<String> &content);

  private:
    [[nodiscard]] Result<bool> whitespace();

    [[nodiscard]] Result<bool> number_literal();

    // [[maybe_unused]] [[nodiscard]] Option<Error> hex_sequence(std::stringstream &stream);

    [[nodiscard]] Result<bool> string_literal();

    [[nodiscard]] Result<bool> operator_tok();

    [[nodiscard]] Result<bool> identifier();
  };
}
