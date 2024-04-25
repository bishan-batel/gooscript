#include "Token.hpp"

#include <stdexcept>

namespace goos::token {
  StringView Token::get_slice() const {
    return slice;
  }
}
