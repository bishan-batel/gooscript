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
#include "token/Identifier.hpp"

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

  char Lexer::curr() const {
    return is_eof() ? '\0' : content.as_ref()->at(position);
  }

  char Lexer::next(const usize n) {
    position += n;
    return curr();
  }

  bool Lexer::is_eof() const { return position >= content.as_ref()->size(); }

  String Lexer::substr(const Range<> range) const {
    auto constraint{[this](auto n) { return std::min(n, content.as_ref()->size()); }};
    return content.as_ref()->substr(
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
    static const Set whitespace_chars{' ', '\t', '\n', '\r'};

    if (not whitespace_chars.contains(curr())) return crab::ok(false);

    next();
    return crab::ok(true);
  }

  Result<bool> Lexer::number_literal() {
    return crab::ok(false);
  }

  Result<bool> Lexer::string_literal() {
    return crab::ok(false);
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

    debug_assert(
      std::ranges::none_of(word, [](auto c) { return token::INVALID_IDENTIFIER_CHARS.contains(c); } ),
      "Failed to correctly parse chars"
    );

    if (auto keyword = identifier_to_keyword(word)) {
      emplace<token::Keyword>(keyword.get_unchecked());
    } else {
      emplace<token::Identifier>(std::move(word));
    }
    return crab::ok(true);
  }
}
