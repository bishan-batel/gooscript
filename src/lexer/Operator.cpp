#include "Operator.hpp"


[[maybe_unused]]
bool lexer::is_assign(lexer::Operator op) {
  return (static_cast<u32>(op) & OperatorFlag::ASSIGN_FLAG);
}

[[maybe_unused]]
bool lexer::is_binary(lexer::Operator op) {
  return (static_cast<u32>(op) & OperatorFlag::BINARY_FLAG);
}

[[maybe_unused]]
bool lexer::is_delimiter(lexer::Operator op) {
  return (static_cast<u32>(op) & OperatorFlag::DELIMITER_FLAG);
}

[[maybe_unused]]
bool lexer::is_parenthetical(lexer::Operator op) {
  const u32 bits = static_cast<u32>(op);
  return (bits & OperatorFlag::PARENTHETICAL_CLOSE_FLAG) or
         (bits & OperatorFlag::PARENTHETICAL_OPEN_FLAG);
}
