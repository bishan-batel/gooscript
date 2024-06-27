//
// Created by bishan_ on 6/21/24.
//

#pragma once
#include <preamble.hpp>

namespace goos::vm::op {
  enum class Code : u8 {
    RETURN,
    CONSTANT,

    // Literals
    NIL,
    UNIT,
    TRUE,
    FALSE,

    SET_LOCAL,
    GET_LOCAL,

    NOP,

    POP,

    // Condtional Logic
    JUMP_IF_FALSE,

    // ARITHMETIC

    // BOOL OPERATIONS
    NOT,
    AND,
    OR,
    EQUALS,

    GREATER_THAN,
    LESS_THAN,
    GREATER_OR_EQUAL_THAN,
    LESS_OR_EQUAL_THAN,

    // Bit Operations
    BIT_AND,
    BIT_OR,
    BIT_XOR,
    SHIFT_LEFT,
    SHIFT_RIGHT,

    ADD,
    SUBTRACT,
    MULIPLY,
    DIVIDE,
    MODULO,
    NEGATE,

    DUP,

    PRINT
  };

  constexpr auto from_byte(const u8 byte) -> Code {
    return std::bit_cast<Code>(byte);
  }

  constexpr auto to_byte(const Code code) -> u8 {
    return std::bit_cast<u8>(code);
  }

  constexpr auto byte_arg_count(const Code code) -> usize {
    switch (code) {
      case Code::JUMP_IF_FALSE:
        return sizeof(usize);

      case Code::GET_LOCAL:
      case Code::SET_LOCAL:
      case Code::CONSTANT:
        return sizeof(u16);

      default:
        return 0;
    }
  }
}
