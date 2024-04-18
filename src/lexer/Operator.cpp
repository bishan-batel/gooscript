#include "Operator.hpp"


bool lexer::is_assign(lexer::Operator op) {
    return (static_cast<u32>(op) & OperatorFlag::ASSIGN_FLAG) != 0;
}

bool lexer::is_binary(lexer::Operator op) {
    return (static_cast<u32>(op) & OperatorFlag::BINARY_FLAG) != 0;
}

bool lexer::is_delimiter(lexer::Operator op) {
    return (static_cast<u32>(op) & OperatorFlag::DELIMITER_FLAG) != 0;
}

bool lexer::is_parenthetical(lexer::Operator op) {
  return (static_cast<u32>(op) & OperatorFlag::PARENTHETICAL_CLOSE_FLAG99999) != 0;
}
