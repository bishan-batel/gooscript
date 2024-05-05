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

namespace goos::parser {
  TokenStream::TokenStream(lexer::TokenList list) : list{std::move(list)} {}

  const token::Token& TokenStream::curr() const {
    if (is_eof()) return eof;
    return list[position];
  }

  const token::Token& TokenStream::next(const usize i) {
    position += i;
    return curr();
  }

  bool TokenStream::is_curr(const token::Token &token) const {
    return curr() == token;
  }

  bool TokenStream::is_curr(const lexer::Operator &token) const {
    return token::Operator{token} == curr();
  }

  bool TokenStream::is_curr(const lexer::Keyword &token) const {
    return token::Keyword{token} == curr();
  }

  bool TokenStream::try_consume(const lexer::Operator op) {
    if (auto tok = try_consume<token::Operator>()) {
      return tok.get_unchecked()->get_op() == op;
    }
    return false;
  }

  bool TokenStream::try_consume(const lexer::Keyword word) {
    if (auto tok = try_consume<token::Keyword>()) {
      return tok.get_unchecked()->get_word() == word;
    }
    return false;
  }

  Result<lexer::Keyword> TokenStream::consume_keyword(const Span<lexer::Keyword> allowed) {
    if (auto word = try_consume<token::Keyword>()) {
      return crab::ok(word.get_unchecked()->get_word());
    }

    std::stringstream msg;

    if (not allowed.empty()) {
      msg << ": ";
      for ([[maybe_unused]] const auto &word: allowed) {
        msg << lexer::KEYWORD_TO_STR_MAP.at(word) << ", ";
      }
    }

    return crab::err(error<err::ExpectedToken>(std::format("Expected a Keyword {}", msg.str()), curr().clone()));
  }

  Result<lexer::Operator> TokenStream::consume_operator(Span<lexer::Operator> allowed) {
    if (auto op = try_consume<token::Operator>()) {
      return crab::ok(op.get_unchecked()->get_op());
    }

    std::stringstream msg;

    if (not allowed.empty()) {
      msg << ": ";
      for ([[maybe_unused]] const auto &op: allowed) {
        msg << '\'' << lexer::OPERATOR_TO_STR_MAP.at(op) << "', ";
      }
    }

    return crab::err(error<err::ExpectedToken>(std::format("Expected an operator {}", msg.str()), curr().clone()));
  }

  Result<String> TokenStream::consume_identifier() {
    if (auto identifier = try_consume<token::Identifier>()) {
      return crab::ok(identifier.get_unchecked()->get_identifier());
    }

    return crab::err(error<err::ExpectedToken>("Expected an Identifier.", curr().clone()));
  }

  bool TokenStream::is_eof() const {
    return position >= list.size();
  }

  err::Error TokenStream::unexpected(String expected) {
    return unexpected(std::move(expected), curr().clone());
  }

  err::Error TokenStream::unexpected(String expected, Box<token::Token> received) {
    return error<err::ExpectedToken>(std::move(expected), std::move(received));
  }
}
