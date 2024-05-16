//
// Created by bishan_ on 4/23/24.
//

#include "Lexer.hpp"

#include <functional>
#include <ranges>
#include <sstream>
#include <token/Keyword.hpp>
#include <token/Operator.hpp>
#include <utility>

#include "Keyword.hpp"
#include "Operator.hpp"
#include "token/Decimal.hpp"
#include "token/Identifier.hpp"
#include "token/Integer.hpp"
#include "token/StringLiteral.hpp"
#include "utils/str.hpp"

namespace goos::lexer {
  auto Error::to_string(const Type type) -> StringView {
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

  Error::Error(const Type type, SourceFile section, const Range<> range)
    : section{std::move(section)},
      slice{range},
      type{type} {}

  auto Error::what() const -> String {
    const String converted{str::convert(section.slice(slice.lower_bound(), slice.upper_bound()))};
    return
        std::format(
          "\"{}\", \n{}",
          converted,
          to_string(type)
        );
  }

  auto Error::get_type() const -> Error::Type { return type; }

  Lexer::Lexer(SourceFile content) : file{std::move(content)} {}

  auto Lexer::error(const Error::Type type, const usize begin) const -> Error {
    return Error{type, file, crab::range(begin, position)};
  }

  auto Lexer::push(Box<Token> token) -> void {
    tokens.push_back(std::move(token));
  }

  auto Lexer::curr() const -> widechar {
    return is_eof() ? L'\0' : file.get_char(position);
  }

  auto Lexer::is_curr(const widechar c) const -> bool {
    return curr() == c;
  }

  auto Lexer::next(const usize n) -> widechar {
    position += n;
    return curr();
  }

  auto Lexer::is_eof() const -> bool { return position >= file.length(); }

  auto Lexer::tokenize(SourceFile content) -> Result<TokenList> {
    Lexer lexer{std::move(content)};

    while (not lexer.is_eof()) {
      const std::array<std::function<Result<bool>()>, 6> passes{
        [&] { return lexer.whitespace(); },
        [&] { return lexer.comment(); },
        [&] { return lexer.operator_tok(); },
        [&] { return lexer.number_literal(); },
        [&] { return lexer.string_literal(); },
        [&] { return lexer.identifier(); }
      };

      bool found_working_pass = false;

      const usize begin{lexer.position};
      for (const auto &pass: passes) {
        const Result<bool> result = pass();

        if (result.is_err()) {
          return err(result.get_err_unchecked());
        }

        if (result.get_unchecked()) {
          found_working_pass = true;
          break;
        }
      }

      if (found_working_pass)
        continue;

      return err(lexer.error(Error::Type::UNKNOWN_CHARACTER, begin));
    }

    return crab::ok(std::move(lexer.tokens));
  }

  auto Lexer::whitespace() -> Result<bool> {
    if (not WHITESPACE_CHARS.contains(curr())) {
      return crab::ok(false);
    }

    while (WHITESPACE_CHARS.contains(curr())) {
      next();
    }
    return crab::ok(true);
  }

  auto Lexer::comment() -> Result<bool> {
    const WideStringView slice{file.slice(crab::range(position, position + 2))};

    constexpr WideStringView begin_multiline{L"/*"}, end_multiline{L"*/"};

    if (is_curr('#') or slice == L"//") {
      while (not is_eof() and not is_curr(L'\n')) next();
      return crab::ok(true);
    }

    if (slice == begin_multiline) {
      while (not is_eof() and file.slice(position, position + end_multiline.length()) != end_multiline)
        next();
      next(end_multiline.length());
      return crab::ok(true);
    }

    return crab::ok(false);
  }

  auto Lexer::number_literal() -> Result<bool> {
    if (not token::DIGIT_CHARS.contains(curr())) {
      return crab::ok(false);
    }

    const usize begin = position;

    constexpr widechar dot{L'.'};

    bool has_dot = false;
    while (token::DIGIT_CHARS.contains(next()) or (not has_dot and is_curr(dot))) {
      has_dot |= is_curr(dot);
    }

    const Range<> text_range{crab::range_inclusive(begin, position)};
    const WideString num_str{file.slice(text_range)};

    if (has_dot) {
      emplace<token::Decimal>(file, text_range, std::stod(num_str));
    } else {
      emplace<token::Integer>(file, text_range, std::stoll(num_str));
    }

    return crab::ok(true);
  }

  auto Lexer::string_literal() -> Result<bool> {
    if (not is_curr(L'"'))
      return crab::ok(false);

    const usize begin = position;

    WideStringStream literal;
    while (not is_eof() and next() != L'"') {
      if (curr() != L'\\') {
        literal << curr();
        continue;
      }
      next();

      #define CASE(x, y)  case x: {literal << y; continue; }
      switch (curr()) {
        CASE(L'0', L'\0')
        CASE(L'n', L'\n')
        CASE(L'r', L'\r')
        CASE(L'v', L'\v')
        CASE(L't', L'\t')
        CASE(L'b', L'\b')
        CASE(L'f', L'\f')
        CASE(L'\'', L'\'')
        CASE(L'"', L'"')
        CASE(L'\\', L'\\')
        case L'\n': { continue; }

        default:
          return err(error(Error::Type::INVALID_ESCAPED_STRING, begin));
      }
      #undef CASE
      // Parse Escape Sequence
    }

    if (not is_curr('"')) {
      return err(error(Error::Type::UNTERMINATED_STRING, begin));
    }

    next();

    emplace<token::StringLiteral>(file, crab::range_inclusive(begin, position), literal.str());

    return crab::ok(true);
  }

  auto Lexer::operator_tok() -> Result<bool> {
    const usize begin = position;

    for (const auto &[string, op]: STR_TO_OPERATOR_MAP) {
      if (string != file.slice(crab::range(begin, begin + string.length()))) {
        continue;
      }

      next(string.length());

      emplace<token::Operator>(file, crab::range(begin, position), op);

      return crab::ok(true);
    }

    return crab::ok(false);
  }

  auto Lexer::identifier() -> Result<bool> {
    if (token::INVALID_IDENTIFIER_CHARS.contains(curr()) or token::DIGIT_CHARS.contains(curr())) {
      return crab::ok(false);
    }

    const usize begin = position;

    auto c = curr();
    while (not token::INVALID_IDENTIFIER_CHARS.contains(c)) {
      c = next();
    }

    const Range<> text_range{crab::range(begin, position)};
    WideString word{file.slice(text_range)};

    if (auto keyword = identifier_to_keyword(word)) {
      emplace<token::Keyword>(file, text_range, keyword.get_unchecked());
    } else {
      emplace<token::Identifier>(file, text_range, std::move(word));
    }
    return crab::ok(true);
  }
}
