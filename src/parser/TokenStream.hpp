//
// Created by bishan_ on 5/3/24.
//

#pragma once
#include <functional>
#include <preamble.hpp>
#include <lexer/Keyword.hpp>
#include <lexer/Operator.hpp>

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
   * @brief A pass that must evaluate to a valid AST expression
   */
  template<typename T>
  using MustEvalPass = std::function<MustEvalResult<T>(TokenStream &)>;

  /**
   * @brief A pass that is able to evaluate to a valid AST expression, must safely handle any errors.
   */
  template<typename T>
  using OptionalPass = std::function<OptionalResult<T>(TokenStream &)>;

  template<typename F>
  concept is_optional_pass = std::is_invocable_v<F, TokenStream&> and requires(F f, TokenStream &stream) {
    OptionalPass<std::remove_reference_t<decltype(f(stream).take_unchecked())>>{f};
  };

  class TokenStream {
    lexer::TokenList list;
    usize position{0};
    token::EndOfFile eof;

  public:
    explicit TokenStream(lexer::TokenList list);

    /**
     * Returns the current token
     */
    [[nodiscard]]
    auto curr() const -> const token::Token&;

    /**
     * @brief Advances by i times and returns the new current token
     * @param i How much to advance
     */
    auto next(usize i = 1) -> const token::Token&;

    /**
     * @brief Queries if the current token is of type T
     * @tparam T Type to check
     */
    template<typename T> requires std::is_base_of_v<token::Token, T>
    [[nodiscard]]
    auto is_curr() const -> bool;

    /**
     * @brief Queries if the current token is equal to the given, note this is strict / deep equality
     */
    [[nodiscard]]
    auto is_curr(const token::Token &token) const -> bool;

    /**
     * @brief Queries if the current token is the specified operator
     */
    [[nodiscard]]
    auto is_curr(const lexer::Operator &op) const -> bool;

    /**
     * @brief Queries if the current token is the specified keyword
     */
    [[nodiscard]]
    auto is_curr(const lexer::Keyword &keyword) const -> bool;

    /**
     * @brief Attempts to take the current token of type T, return it,
     * and then move the stream to the next token. The token will not be consumed
     * if it is not of type T and this function will return None
     */
    template<typename T> requires std::is_base_of_v<token::Token, T>
    [[nodiscard]]
    auto try_consume() -> Option<Ref<T>>;

    /**
     * @brief Attempts to take the current token if it is the given operator, return it,
     * and then move the stream to the next token. If the token is not of type operator or is not
     * the specified operator, it will return None.
     */
    [[nodiscard]]
    auto try_consume(lexer::Operator op) -> bool;

    /**
     * @brief Attempts to take the current token if it is the given operator, return it,
     * and then move the stream to the next token. If the token is not of type operator or is not
     * the specified operator, it will return None.
     */
    [[nodiscard]]
    auto try_consume(lexer::Keyword word) -> bool;

    [[nodiscard]]
    auto consume_keyword(Span<lexer::Keyword> allowed) -> Result<lexer::Keyword>;

    [[nodiscard]]
    auto consume_keyword(lexer::Keyword allowed) -> Result<lexer::Keyword>;

    [[nodiscard]]
    auto consume_operator(Span<lexer::Operator> allowed) -> Result<lexer::Operator>;

    [[nodiscard]]
    auto consume_operator(lexer::Operator allowed) -> Result<lexer::Operator>;

    [[nodiscard]]
    auto consume_identifier() -> Result<WideString>;

    [[nodiscard]] auto is_eof() const -> bool;

    template<typename T, typename... Args> requires
      std::is_base_of_v<err::ErrorBase, T>
    auto error(Args &&... args) -> err::Error;

    auto unexpected(String expected) -> err::Error;

    auto unexpected(String expected, Box<token::Token> received) -> err::Error;
  };

  template<typename T> requires std::is_base_of_v<token::Token, T>
  auto TokenStream::is_curr() const -> bool {
    return crab::ref::cast<T>(curr()).is_some();
  }

  template<typename T> requires std::is_base_of_v<token::Token, T>
  auto TokenStream::try_consume() -> Option<Ref<T>> {
    Option<Ref<T>> casted = crab::ref::cast<T>(curr());
    if (casted) next();
    return casted;
  }

  template<typename T, typename... Args> requires std::is_base_of_v<err::ErrorBase, T>
  auto TokenStream::error(Args &&... args) -> err::Error {
    return err::Error{crab::make_box<T, Args...>(std::forward<Args>(args)...)};
  }
}
