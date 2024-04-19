#pragma once

#include "preamble.hpp"

namespace lexer {
  enum [[maybe_unused]] OperatorFlag : u32 {
    COMPARISON_FLAG = 1 << 30,
    BINARY_FLAG = 1 << 29,
    ASSIGN_FLAG = 1 << 28,
    UNARY_FLAG = 1 << 27,
    DELIMITER_FLAG = 1 << 26,
    PARENTHETICAL = 1 << 25,
    PARENTHETICAL_OPEN_FLAG = 1 << 25,
    PARENTHETICAL_CLOSE_FLAG = 1 << 24,
  };

#define binary(x) ((x) | BINARY_FLAG)
#define assign(x) ((x) | BINARY_FLAG | ASSIGN_FLAG)

#define compare(x) ((x) | BINARY_FLAG | COMPARISON_FLAG)
#define unary(x) ((x) | UNARY_FLAG)
#define delimiter(x) ((x) | DELIMITER_FLAG)

#define paren(x, closed) ((x) | PARENTHETICAL_##closed##_FLAG)

  enum class [[maybe_unused]] Operator : u32 {
    ASSIGN = assign(0),
    MOD_ASSIGN = assign(1),
    MUL_ASSIGN = assign(2),
    DIV_ASSIGN = assign(3),
    ADD_ASSIGN = assign(4),
    SUB_ASSIGN = assign(5),
    BIT_OR_ASSIGN = assign(6),
    BIT_AND_ASSIGN = assign(7),
    XOR = binary(8),
    BIT_OR = binary(9),
    BIT_AND = binary(10),
    AND = binary(11),
    OR = binary(12),
    NOT = 13 | UNARY_FLAG,
    EQUALS = compare(14),
    NOT_EQUALS = compare(15),
    GREATER = compare(16),
    LESS = compare(17),
    GREATER_OR_EQUALS = compare(18),
    LESS_OR_EQUALS = compare(19),
    SHIFT_LEFT = binary(20),
    SHIFT_RIGHT = binary(21),
    ADD = binary(22),
    SUB = binary(23),
    MUL = binary(24),
    DIV = binary(25),
    MOD = binary(26),
    ARROW = delimiter(27),
    SEMICOLON = delimiter(28),
    COMMA = delimiter(29),

    PAREN_OPEN = paren(30, OPEN),
    PAREN_CLOSE = paren(30, CLOSE),

    BRACKET_OPEN = paren(31, OPEN),
    BRACKET_CLOSE = paren(31, CLOSE),

    CURLY_OPEN = paren(32, OPEN),
    CURLY_CLOSE = paren(32, CLOSE),
  };

  [[maybe_unused]]
  bool is_binary(Operator op);

  [[maybe_unused]]
  bool is_assign(Operator op);

  [[maybe_unused]]
  bool is_delimiter(Operator op);

  [[maybe_unused]]
  bool is_parenthetical(Operator op);

#undef binary
#undef assign
#undef compare
#undef unary
#undef delimiter
#undef paren
} // namespace lexer
