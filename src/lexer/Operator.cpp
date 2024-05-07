#include "Operator.hpp"

namespace goos::lexer {
  [[maybe_unused]]
  auto is_assign(Operator op) -> bool {
    return (static_cast<u32>(op) & ASSIGN_FLAG);
  }

  [[maybe_unused]]
  auto is_binary(Operator op) -> bool {
    return (static_cast<u32>(op) & BINARY_FLAG);
  }

  [[maybe_unused]]
  auto is_delimiter(Operator op) -> bool {
    return (static_cast<u32>(op) & DELIMITER_FLAG);
  }

  [[maybe_unused]]
  auto is_parenthetical(Operator op) -> bool {
    const u32 bits = static_cast<u32>(op);
    return bits & PARENTHETICAL_CLOSE_FLAG or
           bits & PARENTHETICAL_OPEN_FLAG;
  }
}
