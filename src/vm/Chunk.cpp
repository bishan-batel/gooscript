//
// Created by bishan_ on 6/21/24.
//

#include "Chunk.hpp"

#include <stdexcept>

#include <crab/debug.hpp>
#include <fmt/color.h>

namespace goos::vm {
  Chunk::Chunk(WideString name)
    : name{std::move(name)} {
    constants.reserve(std::numeric_limits<u8>::max());
  }

  auto Chunk::define_constant(const Value value) -> u16 {
    constants.push_back(value);
    return constants.size() - 1;
  }

  auto Chunk::get_constant(const usize id) const -> const Value& {
    return constants[id];
  }

  auto Chunk::write_byte(const u8 byte) -> void {
    bytes.push_back(byte);
  }

  auto Chunk::operator<<(const u8 byte) -> Chunk& {
    write_byte(byte);
    return *this;
  }

  auto Chunk::operator<<(const op::Code code) -> Chunk& {
    write_byte(to_byte(code));
    return *this;
  }

  auto Chunk::dissassemble_instruction(const usize i) const -> void {
    using namespace fmt;

    switch (op::from_byte(get_byte(i))) {
      case op::Code::RETURN: {
        println("{}", styled("ret", fg(color::coral)));
        return;
      }
      case op::Code::CONSTANT: {
        println(
          "{} {}",
          styled("const", fg(color::cornflower_blue)),
          styled(get_bytes<u16>(i + 1), fg(color::light_golden_rod_yellow))
        );
        return;
      }

      case op::Code::NIL: {
        println("{}", styled("nil", fg(color::coral)));
        return;
      }

      case op::Code::UNIT: {
        println("{}", styled("unit", fg(color::coral)));
        return;
      }

      case op::Code::TRUE: {
        println("{}", styled("true", fg(color::coral)));
        return;
      }
      case op::Code::FALSE: {
        println("{}", styled("false", fg(color::coral)));
        return;
      }
      case op::Code::SET_LOCAL: {
        println(
          "{} {}",
          styled("store", fg(color::cornflower_blue)),
          styled(get_bytes<u16>(i + 1), fg(color::light_golden_rod_yellow))
        );
        return;
      }
      case op::Code::GET_LOCAL: {
        println(
          "{} {}",
          styled("load", fg(color::cornflower_blue)),
          styled(get_bytes<u16>(i + 1), fg(color::light_golden_rod_yellow))
        );
        return;
      }
      case op::Code::POP: {
        println("{}", styled("pop", fg(color::coral)));
        return;
      }
      case op::Code::JUMP_IF_FALSE: {
        println(
          "{} {}",
          styled("jmpnif", fg(color::cornflower_blue)),
          styled(get_bytes<u16>(i + 1), fg(color::light_golden_rod_yellow))
        );
        return;
      }
      case op::Code::NOT: ; {
          println("{}", styled("not", fg(color::coral)));
          return;
        }
      case op::Code::AND: {
        println("{}", styled("and", fg(color::coral)));
        return;
      }
      case op::Code::OR: {
        println("{}", styled("or", fg(color::coral)));
        return;
      }
      case op::Code::EQUALS: {
        println("{}", styled("eq", fg(color::coral)));
        return;
      }
      case op::Code::GREATER_THAN: {
        println("{}", styled("gt", fg(color::coral)));
        return;
      }
      case op::Code::LESS_THAN: {
        println("{}", styled("lt", fg(color::coral)));
        return;
      }
      case op::Code::GREATER_OR_EQUAL_THAN: {
        println("{}", styled("gte", fg(color::coral)));
        return;;
      }
      case op::Code::LESS_OR_EQUAL_THAN: {
        println("{}", styled("lte", fg(color::coral)));
        return;
      }
      case op::Code::BIT_AND: {
        println("{}", styled("bitand", fg(color::coral)));
        return;
      }
      case op::Code::BIT_OR: {
        println("{}", styled("bitor", fg(color::coral)));
        return;
      }
      case op::Code::BIT_XOR: {
        println("{}", styled("xor", fg(color::coral)));
        return;
      }
      case op::Code::SHIFT_LEFT: {
        println("{}", styled("shiftl", fg(color::coral)));
        return;
      }
      case op::Code::SHIFT_RIGHT: {
        println("{}", styled("shiftr", fg(color::coral)));
        return;
      }
      case op::Code::ADD: {
        println("{}", styled("add", fg(color::coral)));
        return;
      }
      case op::Code::SUBTRACT: {
        println("{}", styled("sub", fg(color::coral)));
        return;
      }
      case op::Code::MULIPLY: {
        println("{}", styled("mul", fg(color::coral)));
        return;
      }
      case op::Code::DIVIDE: {
        println("{}", styled("div", fg(color::coral)));
        return;
      }
      case op::Code::MODULO: {
        println("{}", styled("mod", fg(color::coral)));
        return;
      }
      case op::Code::NEGATE: {
        println("{}", styled("neg", fg(color::coral)));
        return;
      }
      case op::Code::NOP:
        println("{}", styled("nop", fg(color::coral)));
        return;
      case op::Code::PRINT:
        println("{}", styled("print", fg(color::spring_green)));
        break;
      case op::Code::DUP:
        println("{}", styled("dup", fg(color::coral)));
        break;
    }
  }

  auto Chunk::dissassemble() const -> void {
    const usize length = bytes.size();

    usize instruction = 0;

    using namespace fmt;

    for (usize i = 0; i < length;) {
      print(
        "{}{:#04x}{} {:04}{} ",
        styled('[', fg(color::gray)),
        styled(i, fg(color::indian_red)),
        styled(":", fg(color::gray)),
        styled(instruction, fg(color::green_yellow)),
        styled(']', fg(color::gray))
      );

      dissassemble_instruction(i);
      i += byte_arg_count(op::from_byte(get_byte(i))) + 1;
      instruction++;
    }
  }

  auto Chunk::get_byte(const usize i) const -> u8 { return bytes.at(i); }

  auto Chunk::label() const -> usize {
    return byte_count();
  }
}
