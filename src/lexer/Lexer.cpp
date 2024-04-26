//
// Created by bishan_ on 4/23/24.
//

#include "Lexer.hpp"

#include <functional>
#include <bits/ranges_algo.h>
#include <fmt/compile.h>
#include <token/Keyword.hpp>
#include <token/Operator.hpp>

#include "Keyword.hpp"
#include "Operator.hpp"
#include "token/Decimal.hpp"
#include "token/Identifier.hpp"
#include "token/Integer.hpp"
#include "token/StringLiteral.hpp"

namespace goos::lexer {
  StringView Error::to_string(const Type type) {
    switch (type) {
      case Type::UNTERMINATED_STRING:
        return "Unterminated String";
      case Type::INVALID_ESCAPED_STRING:
        return "Invalid Escape Sequence";
      case Type::UNKNOWN_CHARACTER:
        return "Unknown Character";
    }

    return {};
  }

  Error::Error(const Type type, const Rc<String> &section, const Range<> range)
    : section{section},
      slice{range},
      type{type},
      msg{
        std::format(
          "\"{}\", \n{}",
          section.as_ref()->substr(range.lower_bound(), range.upper_bound() - range.lower_bound()),
          to_string(type)
        )
      } {}

  StringView Error::what() const { return msg; }

  Error::Type Error::get_type() const { return type; }

  Lexer::Lexer(const Rc<String> &content) : content{content} {}

  Error Lexer::error(const Error::Type type, const usize begin) const {
    return Error{type, content, crab::range(begin, std::min(content.as_ref()->size(), position))};
  }

  void Lexer::push(Box<Token> token) {
    tokens.push_back(std::move(token));
  }

  char Lexer::curr() const {
    return is_eof() ? '\0' : content.as_ref()->at(position);
  }

  bool Lexer::is_curr(const char c) const {
    return curr() == c;
  }

  char Lexer::next(const usize n) {
    position += n;
    return curr();
  }

  bool Lexer::is_eof() const { return position >= content.as_ref()->size(); }

  StringView Lexer::substr(const Range<> range) const {
    auto constraint{[this](auto n) { return std::min(n, content.as_ref()->size()); }};
    return StringView{*content.as_ref()}.substr(
      constraint(range.lower_bound()),
      constraint(range.upper_bound()) - constraint(range.lower_bound())
    );
  }

  Result<TokenList> Lexer::tokenize(const Rc<String> &content) {
    Lexer lexer{content};

    while (not lexer.is_eof()) {
      const std::array<std::function<Result<bool>()>, 5> passes{
        [&] { return lexer.whitespace(); },
        [&] { return lexer.number_literal(); },
        [&] { return lexer.string_literal(); },
        [&] { return lexer.operator_tok(); },
        [&] { return lexer.identifier(); }
      };

      bool found = false;

      const usize begin{lexer.position};
      for (const auto &pass: passes) {
        const Result<bool> result = pass();

        if (result.is_err()) {
          return err(result.get_err_unchecked());
        }

        if (result.get_unchecked()) {
          found = true;
          break;
        }
      }

      if (found) continue;

      return err(lexer.error(Error::Type::UNKNOWN_CHARACTER, begin));
    }

    return crab::ok(std::move(lexer.tokens));
  }

  Result<bool> Lexer::whitespace() {

    if (not WHITESPACE_CHARS.contains(curr())) return crab::ok(false);

    next();
    return crab::ok(true);
  }

  Result<bool> Lexer::number_literal() {
    if (not std::isdigit(curr())) {
      return crab::ok(false);
    }

    const usize begin = position;

    bool has_dot = false;
    while (std::isdigit(next()) or (not has_dot and is_curr('.'))) {
      has_dot |= is_curr('.');
    }

    const String num_str{substr(crab::range_inclusive(begin, position))};

    if (has_dot) {
      emplace<token::Decimal>(std::stod(num_str));
    } else {
      emplace<token::Integer>(std::stoll(num_str));
    }

    return crab::ok(true);
  }

  Result<bool> Lexer::string_literal() {
    if (not is_curr('"')) return crab::ok(false);

    const usize begin = position + 1;
    while (not is_eof() and next() != '"') {}

    if (not is_curr('"')) {
      return err(error(Error::Type::UNTERMINATED_STRING, begin));
    }

    const StringView literal = substr(crab::range(begin, position));
    next();

    emplace<token::StringLiteral>(literal);

    return crab::ok(true);
  }

  Result<bool> Lexer::operator_tok() {
    const usize begin = position;

    for (const auto &[string, op]: STR_TO_OPERATOR_MAP) {
      if (string != substr(crab::range_inclusive(begin, position))) {
        continue;
      }

      next(string.length());

      emplace<token::Operator>(op);

      return crab::ok(true);
    }

    return crab::ok(false);
  }

  Result<bool> Lexer::identifier() {
    if (token::INVALID_IDENTIFIER_CHARS.contains(curr()) or std::isdigit(curr())) {
      return crab::ok(false);
    }

    const usize begin = position;

    auto c = curr();
    while (not token::INVALID_IDENTIFIER_CHARS.contains(c)) {
      c = next();
    }

    String word{substr(crab::range(begin, position))};

    if (auto keyword = identifier_to_keyword(word)) {
      emplace<token::Keyword>(keyword.get_unchecked());
    } else {
      emplace<token::Identifier>(std::move(word));
    }
    return crab::ok(true);
  }
}
