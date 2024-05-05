//
// Created by bishan_ on 5/3/24.
//

#pragma once
#include <functional>
#include <preamble.hpp>
#include <lexer/Keyword.hpp>
#include <lexer/Operator.hpp>
#include <token/Operator.hpp>

#include "Error.hpp"
#include "ast/Statement.hpp"
#include "lexer/Lexer.hpp"
#include "token/EndOfFile.hpp"

namespace goos::parser {
  class TokenStream;

  template<typename T>
  using Result = Result<T, err::Error>;

  template<typename T>
  using MustEvalResult = Result<Box<T>>;

  template<typename T>
  using OptionalResult = Result<Option<Box<T>>>;

  /**
   * A pass that must evaluate to a valid AST expression
   */
  template<typename T>
  using MustEvalPass = std::function<MustEvalResult<T>(TokenStream &)>;

  template<typename T>
  using OptionalPass = std::function<OptionalResult<T>(TokenStream &)>;

  class TokenStream {
    lexer::TokenList list;
    usize position{0};
    token::EndOfFile eof;

  public:
    inline static const Vec<OptionalPass<Box<ast::Statement>>> TOP_LEVEL_PASSES;

    explicit TokenStream(lexer::TokenList list);

    [[nodiscard]] const token::Token& curr() const;

    const token::Token& next(usize i = 1);

    template<typename T> requires std::is_base_of_v<token::Token, T>
    [[nodiscard]] bool is_curr() const {
      return crab::ref::cast<T>(curr()).is_some();
    }

    [[nodiscard]] bool is_curr(const token::Token &token) const;

    [[nodiscard]] bool is_curr(const lexer::Operator &token) const;

    [[nodiscard]] bool is_curr(const lexer::Keyword &token) const;

    template<typename T> requires std::is_base_of_v<token::Token, T>
    [[nodiscard]] Option<Ref<T>> try_consume() {
      Option<Ref<T>> casted = crab::ref::cast<T>(curr());
      if (casted) next();
      return casted;
    }

    [[nodiscard]] bool try_consume(lexer::Operator op);

    [[nodiscard]] bool try_consume(lexer::Keyword word);

    [[nodiscard]]
    Result<lexer::Keyword> consume_keyword();

    [[nodiscard]]
    Result<String> consume_operator();

    [[nodiscard]]
    Result<String> consume_identifier();

    [[nodiscard]] bool is_eof() const;

    template<typename T, typename... Args> requires
      std::is_base_of_v<err::ErrorBase, T>
    err::Error error(Args &&... args) {
      return err::Error{crab::make_box<T, Args...>(std::forward<Args>(args)...)};
    }
  };
}
