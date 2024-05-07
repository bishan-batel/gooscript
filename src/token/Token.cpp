#include "Token.hpp"

#include <stdexcept>

#include "utils/str.hpp"

namespace goos::token {
  StringView Token::get_slice() const {
    return slice;
  }

  std::ostream& operator<<(std::ostream &os, const Token &token) {
    return os << str::convert(token.to_string());
  }
}
