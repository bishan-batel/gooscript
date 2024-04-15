#pragma once
#include "keyword.hpp"
#include <preamble.hpp>
#include <box.hpp>

class Token {
  StringView slice;

protected:
  Token(const Token &) = default;

public:
  Token() = default;
  Token(Token &&) = delete;
  virtual ~Token() = default;

  void operator=(const Token &) = delete;
  void operator=(Token &&) = delete;

  [[nodiscard]] virtual Box<Token> clone() const = 0;

  [[nodiscard]] StringView get_slice() const { return slice; }
};

class KeywordToken : public Token {
  Keyword keyword;

public:
  explicit KeywordToken(Keyword keyword);

  [[nodiscard]] Box<Token> clone() const override;

  [[nodiscard]] Keyword get_word() const { return keyword; }
};
