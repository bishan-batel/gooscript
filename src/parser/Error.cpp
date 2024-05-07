//
// Created by bishan_ on 5/3/24.
//

#include <fmt/format.h>
#include "Error.hpp"

#include <codecvt>

namespace goos::parser::err {
  ErrorBase::ErrorBase(Box<token::Token> token) : token{std::move(token)} {}

  const token::Token& ErrorBase::where() const { return token; }

  ExpectedToken::ExpectedToken(String expected_type, Box<token::Token> receieved)
    : ErrorBase{std::move(receieved)}, expected_type{std::move(expected_type)} {}

  String ExpectedToken::what() const {
    const String normal_str = std::wstring_convert<std::codecvt_utf8<widechar>>().to_bytes(where().to_string());
    return fmt::format("Expected {}, Received '{}'", expected_type, normal_str);
  }

  const String& ExpectedToken::get_expected() const { return expected_type; }
}
