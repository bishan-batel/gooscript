#include "Operator.hpp"

namespace goos::lexer {
  [[maybe_unused]]
  bool is_assign(Operator op) {
    return (static_cast<u32>(op) & ASSIGN_FLAG);
  }

  [[maybe_unused]]
  bool is_binary(Operator op) {
    return (static_cast<u32>(op) & BINARY_FLAG);
  }

  [[maybe_unused]]
  bool is_delimiter(Operator op) {
    return (static_cast<u32>(op) & DELIMITER_FLAG);
  }

  [[maybe_unused]]
  bool is_parenthetical(Operator op) {
    const u32 bits = static_cast<u32>(op);
    return bits & PARENTHETICAL_CLOSE_FLAG or
           bits & PARENTHETICAL_OPEN_FLAG;
  }
}
