//
// Created by bishan_ on 5/3/24.
//

#include "TokenStream.hpp"

#include <ranges>
#include <sstream>
#include <token/Keyword.hpp>
#include <token/Operator.hpp>

#include "token/EndOfFile.hpp"
#include "token/Identifier.hpp"
#include "utils/str.hpp"

namespace goos::parser {
  TokenStream::TokenStream(lexer::TokenList list) : list{std::move(list)} {}

  auto TokenStream::curr() const -> const token::Token& {
    return get_token(position);
  }

  auto TokenStream::next(const usize i) -> const token::Token& {
    position += i;
    return curr();
  }

  auto TokenStream::is_curr(const token::Token &token) const -> bool {
    return curr() == token;
  }

  auto TokenStream::is_curr(const lexer::Operator &op) const -> bool {
    return token::Operator{op} == curr();
  }

  auto TokenStream::is_curr(const lexer::Keyword &keyword) const -> bool {
    return token::Keyword{keyword} == curr();
  }

  auto TokenStream::try_consume(const lexer::Operator op) -> bool {
    if (auto tok = try_consume<token::Operator>()) {
      const bool valid = tok.get_unchecked()->get_op() == op;
      if (not valid) position--;
      return valid;
    }
    return false;
  }

  auto TokenStream::try_consume(const lexer::Keyword word) -> bool {
    if (auto tok = try_consume<token::Keyword>()) {
      const bool valid = tok.get_unchecked()->get_word() == word;
      if (not valid) position--;
      return valid;
    }
    return false;
  }

  auto TokenStream::consume_keyword(const Span<lexer::Keyword> allowed) -> Result<lexer::Keyword> {
    if (auto word = try_consume<token::Keyword>()) {
      return crab::ok(word.get_unchecked()->get_word());
    }

    WideStringStream msg;

    if (not allowed.empty()) {
      msg << ": ";
      for ([[maybe_unused]] const auto &word: allowed) {
        msg << lexer::KEYWORD_TO_STR_MAP.at(word) << ", ";
      }
    }

    return crab::err(
      error<err::ExpectedToken>(std::format("Expected a Keyword {}", str::convert(msg.str())), curr().clone())
    );
  }

  auto TokenStream::consume_keyword(const lexer::Keyword allowed) -> Result<lexer::Keyword> {
    std::array arr{allowed};
    return consume_keyword(std::span{arr});
  }

  auto TokenStream::consume_operator(const lexer::Operator allowed) -> Result<lexer::Operator> {
    std::array arr{allowed};
    return consume_operator(std::span{arr});
  }

  auto TokenStream::consume_operator(const Span<lexer::Operator> allowed) -> Result<lexer::Operator> {
    if (auto op = try_consume<token::Operator>()) {
      return crab::ok(op.get_unchecked()->get_op());
    }

    WideStringStream msg;

    if (not allowed.empty()) {
      msg << ": ";
      for ([[maybe_unused]] const auto &op: allowed) {
        msg << '\'' << lexer::OPERATOR_TO_STR_MAP.at(op) << "', ";
      }
    }

    return crab::err(
      error<err::ExpectedToken>(std::format("Expected an operator {}", str::convert(msg.str())), curr().clone())
    );
  }

  auto TokenStream::consume_identifier() -> Result<WideString> {
    if (auto identifier = try_consume<token::Identifier>()) {
      return crab::ok(identifier.get_unchecked()->get_identifier());
    }

    return crab::err(error<err::ExpectedToken>("Expected an Identifier.", curr().clone()));
  }

  auto TokenStream::is_eof() const -> bool {
    return position >= list.size();
  }

  auto TokenStream::unexpected(String expected) -> err::Error {
    return unexpected(std::move(expected), curr().clone());
  }

  auto TokenStream::unexpected(String expected, Box<token::Token> received) -> err::Error {
    return error<err::ExpectedToken>(std::move(expected), std::move(received));
  }

  auto TokenStream::get_position() const -> usize {
    return position;
  }

  auto TokenStream::get_token(const usize position) const -> const token::Token& {
    if (position < list.size()) return list[position];
    return eof;
  }

  auto TokenStream::get_string(const Range<> tok_range) const -> WideString {
    WideStringStream stream{};

    for (const usize i: tok_range) {
      stream << get_token(i).get_slice();
    }

    return stream.str();
  }

  auto TokenStream::backpedal(usize i) -> void {
    position -= i;
  }
}
