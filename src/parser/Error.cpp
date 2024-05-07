//
// Created by bishan_ on 5/3/24.
//

#include <fmt/format.h>
#include "Error.hpp"

#include <codecvt>

namespace goos::parser::err {
  ErrorBase::ErrorBase(Box<token::Token> token) : token{std::move(token)} {}

  auto ErrorBase::where() const -> const token::Token& { return token; }

  ExpectedToken::ExpectedToken(String expected_type, Box<token::Token> receieved)
    : ErrorBase{std::move(receieved)}, expected_type{std::move(expected_type)} {}

  auto ExpectedToken::what() const -> String {
    const String normal_str = std::wstring_convert<std::codecvt_utf8<widechar>>().to_bytes(where().to_string());
    return fmt::format("Expected {}, Received '{}'", expected_type, normal_str);
  }

  auto ExpectedToken::get_expected() const -> const String& { return expected_type; }
}
