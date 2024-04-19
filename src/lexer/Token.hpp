#pragma once
#include "Keyword.hpp"
#include "Operator.hpp"
#include "libs/crabpp/inc/box.hpp"
#include "libs/crabpp/inc/preamble.hpp"

// fn main() 
// j;alsjdf;a
// list [ keyword('fn'), identifier('main'), openParen, closeParen ] 


namespace lexer {
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

class OperatorToken : public Token {
  Operator op;

public:
  explicit OperatorToken(Operator op);

  [[nodiscard]] Box<Token> clone() const override;

  [[nodiscard]] Operator get_op() const;
};

} // namespace lexer
