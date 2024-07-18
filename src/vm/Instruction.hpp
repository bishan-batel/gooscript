//
// Created by bishan_ on 6/21/24.
//

#pragma once
#include <preamble.hpp>
#include <utility>

namespace goos::vm::op {
  enum class Code : u8 {
    RETURN,
    CONSTANT,

    // Literals
    NIL,
    UNIT,
    TRUE,
    FALSE,

    SET,
    GET,

    NOP,

    POP,
    SWAP,

    // Condtional Logic
    JUMP_IF_FALSE,
    JUMP,

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

  constexpr auto from_byte(const u8 byte) -> Code { return std::bit_cast<Code>(byte); }
  constexpr auto to_byte(const Code code) -> u8 { return std::bit_cast<u8>(code); }

  constexpr auto stack_influence(const Code code) -> i64 {
    switch (code) {
        // case Code::RETURN:

      case Code::DUP:
      case Code::GET:
      case Code::CONSTANT:
      case Code::NIL:
      case Code::UNIT:
      case Code::TRUE:
      case Code::FALSE:
        return -1;

      case Code::POP:

      case Code::JUMP_IF_FALSE:
      case Code::RETURN:
      case Code::SET:

      case Code::AND:
      case Code::OR:
      case Code::EQUALS:
      case Code::GREATER_THAN:
      case Code::LESS_THAN:
      case Code::GREATER_OR_EQUAL_THAN:
      case Code::LESS_OR_EQUAL_THAN:
      case Code::BIT_AND:
      case Code::BIT_OR:
      case Code::BIT_XOR:
      case Code::SHIFT_LEFT:
      case Code::SHIFT_RIGHT:
      case Code::ADD:
      case Code::SUBTRACT:
      case Code::MULIPLY:
      case Code::DIVIDE:
      case Code::MODULO:
        return 1;

      case Code::NOP:
      case Code::NOT:
      case Code::NEGATE:
      case Code::PRINT:
      case Code::JUMP:
      case Code::SWAP:
        return 0;
    }

    std::unreachable();
  }

  constexpr auto byte_arg_count(const Code code) -> u64 {
    switch (code) {
      case Code::JUMP_IF_FALSE:
      case Code::JUMP:
      case Code::GET:
      case Code::SET:
        return sizeof(u64);

      case Code::CONSTANT:
        return sizeof(u16);

      default:
        return 0;
    }

    std::unreachable();
  }
} // namespace goos::vm::op
