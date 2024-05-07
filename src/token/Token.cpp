#include "Token.hpp"

#include "utils/str.hpp"

namespace goos::token {
  auto Token::get_slice() const -> StringView {
    return slice;
  }

  auto operator<<(std::ostream &os, const Token &token) -> std::ostream& {
    return os << str::convert(token.to_string());
  }
}
