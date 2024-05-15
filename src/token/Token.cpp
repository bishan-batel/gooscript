#include "Token.hpp"

#include "utils/str.hpp"

namespace goos::token {
  Token::Token(SourceFile file, const Range<> range) : file{std::move(file)}, range{range} {}

  auto Token::get_file() const -> const SourceFile& {
    return file;
  }

  auto Token::get_range() const -> const Range<>& {
    return range;
  }

  auto Token::get_slice() const -> WideString {
    return file.slice(range);
  }

  auto Token::get_line_and_column() const -> std::pair<usize, usize> {
    usize line = 1, column = 1;
    for (usize c = 0; c < range.lower_bound(); c++) {
      column++;

      if (get_file().get_char(c) == '\n') {
        column = 1;
        line++;
      }
    }

    return std::make_pair(line, column);
  }

  auto operator<<(std::ostream &os, const Token &token) -> std::ostream& {
    return os << str::convert(token.to_string());
  }
}
