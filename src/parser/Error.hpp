//
// Created by bishan_ on 5/3/24.
//

#pragma once
#include <range.hpp>
#include <rc.hpp>
#include <result.hpp>
#include <token/Token.hpp>

namespace goos::parser::err {
  class ErrorBase {
    Box<token::Token> token;

  public:
    explicit ErrorBase(Box<token::Token> token);

    ErrorBase(const ErrorBase &) = delete;

    ErrorBase(ErrorBase &&) = delete;

    virtual ~ErrorBase() = default;

    ErrorBase& operator=(const ErrorBase &) = delete;

    ErrorBase& operator=(ErrorBase &&) = delete;

    /**
     * The token that was being read when this error was returned.
     */
    [[nodiscard]] const token::Token& where() const;

    /**
     * String message of the error, for debug purpouses
     */
    [[nodiscard]] virtual String what() const = 0;
  };

  class Error final : crab::Error {
    Box<ErrorBase> error;

  public:
    explicit Error(Box<ErrorBase> error)
      : error{std::move(error)} {}

    [[nodiscard]] String what() const override { return error->what(); }
  };

  class ExpectedToken final : public ErrorBase {
    String expected_type;

  public:
    ExpectedToken(String expected_type, Box<token::Token> receieved);

    [[nodiscard]] String what() const override;

    [[nodiscard]] const String& get_expected() const;
  };
}
