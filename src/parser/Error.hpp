//
// Created by bishan_ on 5/3/24.
//

#pragma once
#include <range.hpp>
#include <result.hpp>
#include <token/Token.hpp>

#include "source/SourceFile.hpp"

namespace goos::parser::err {
  class ErrorBase {
    Box<token::Token> token;

  public:
    explicit ErrorBase(Box<token::Token> token);

    ErrorBase(const ErrorBase &) = delete;

    ErrorBase(ErrorBase &&) = delete;

    virtual ~ErrorBase() = default;

    auto operator=(const ErrorBase &) -> ErrorBase& = delete;

    auto operator=(ErrorBase &&) -> ErrorBase& = delete;

    /**
     * The token that was being read when this error was returned.
     */
    [[nodiscard]] auto where() const -> const token::Token&;

    /**
     * String message of the error, for debug purpouses
     */
    [[nodiscard]] virtual auto what() const -> String = 0;
  };

  class Error final : crab::Error {
    Box<ErrorBase> error;

  public:
    explicit Error(Box<ErrorBase> error)
      : error{std::move(error)} {}

    [[nodiscard]] auto get() const -> const ErrorBase& { return error; };

    [[nodiscard]] auto what() const -> String override { return error->what(); }
  };

  class ExpectedToken final : public ErrorBase {
    String expected_type;

  public:
    ExpectedToken(String expected_type, Box<token::Token> receieved);

    [[nodiscard]] auto what() const -> String override;

    [[nodiscard]] auto get_expected() const -> const String&;
  };

  class ExpectedExpression final : public ErrorBase {
    Box<token::Token> from;

  public:
    ExpectedExpression(Box<token::Token> from, Box<token::Token> to);

    [[nodiscard]] auto what() const -> String override;
  };
}
