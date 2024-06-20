//
// Created by bishan_ on 5/3/24.
//

#include "Error.hpp"
#include <fmt/format.h>

#include "utils/str.hpp"

namespace goos::parser::err {
  ErrorBase::ErrorBase(Rc<token::Token> token) : token{std::move(token)} {}

  auto ErrorBase::where() const -> const token::Token& { return token; }

  ExpectedToken::ExpectedToken(String expected_type, Rc<token::Token> receieved)
    : ErrorBase{std::move(receieved)}, expected_type{std::move(expected_type)} {}

  auto ExpectedToken::what() const -> String {
    const auto [line, column] = where().get_line_and_column();
    return fmt::format(
      "Expected {}, Received '{}' ({}:{})",
      expected_type,
      str::convert(where().to_string()),
      line,
      column
    );
  }

  auto ExpectedToken::get_expected() const -> const String& { return expected_type; }

  ExpectedExpression::ExpectedExpression(Rc<token::Token> from, Rc<token::Token> to)
    : ErrorBase{(std::move(to))}, from{std::move(from)} {}

  auto ExpectedExpression::what() const -> String {
    const auto [line, column] = where().get_line_and_column();
    return fmt::format(
      "Expected Expression, but received Statement: '{}' ({}:{})",
      str::convert(where().to_string()),
      line,
      column
    );
  }
} // namespace goos::parser::err
