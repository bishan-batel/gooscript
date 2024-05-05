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
    String msg;

  public:
    explicit ErrorBase(Box<token::Token> token, String msg);

    [[nodiscard]] const token::Token& where() const;

    [[nodiscard]] StringView what() const;
  };

  class Error final : crab::Error {
    Box<ErrorBase> error;

  public:
    explicit Error(Box<ErrorBase> error)
      : error{std::move(error)} {}

  private:
    [[nodiscard]] StringView what() const override { return error->what(); }
  };

  class ExpectedToken final : public ErrorBase {
    String expected_type;

  public:
    ExpectedToken(String expected_type, Box<token::Token> receieved);

    [[nodiscard]] const String& get_expected() const;
  };
}
