//
// Created by bishan_ on 5/3/24.
//

#include <fmt/format.h>
#include "Error.hpp"

namespace goos::parser::err {
  ErrorBase::ErrorBase(Box<token::Token> token, String msg) : token{std::move(token)}, msg{std::move(msg)} {}

  const token::Token& ErrorBase::where() const { return token; }

  StringView ErrorBase::what() const { return msg; }

  ExpectedToken::ExpectedToken(String expected_type, Box<token::Token> receieved)
    : ErrorBase{
      std::move(receieved),
      fmt::format("Expected {}, Received {}", expected_type, receieved->to_string())
    } {}

  const String& ExpectedToken::get_expected() const { return expected_type; }
}
