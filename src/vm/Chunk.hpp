//
// Created by bishan_ on 6/21/24.
//

#pragma once

#include <crab/debug.hpp>
#include "Instruction.hpp"

#include <bit>
#include "Value.hpp"

namespace goos::vm {
  class Chunk {
    WideString name;
    Vec<u8> bytes{};
    Vec<Value> constants{};

  public:
    explicit Chunk(WideString name);

    // Writing instructions
    template<std::convertible_to<u8>... Bytes>
    auto write_instruction(op::Code code, Bytes... bytes) -> void;

    template<std::copyable T>
      requires(not std::same_as<T, u8>)
    auto write_instruction(op::Code code, T obj) -> void;

    auto define_constant(Value value) -> u16;

    [[nodiscard]] auto get_constant(usize id) const -> const Value &;

    auto write_byte(u8 byte) -> void;

    template<std::convertible_to<u8>... Bytes>
    auto write_bytes(Bytes... bytes) -> void;

    auto operator<<(u8 byte) -> Chunk &;

    auto operator<<(op::Code code) -> Chunk &;

    // Dissassembly
    auto dissassemble_instruction(usize i) const -> void;

    auto dissassemble() const -> void;

    [[nodiscard]] auto get_byte(usize i) const -> u8;

    template<std::copyable T>
    [[nodiscard]] auto get_bytes(usize i) const -> T;

    [[nodiscard]] auto byte_count() const -> usize { return bytes.size(); }

    [[nodiscard]] auto label() const -> usize;
  };

  template<std::convertible_to<u8>... Bytes>
  auto Chunk::write_instruction(const op::Code code, Bytes... bytes) -> void {
    debug_assert(op::byte_arg_count(code) == sizeof...(Bytes), "Incorrect number of arguments");
    //(I op ... op E)
    std::ignore = ((*this << code) << ... << bytes);
  }

  template<std::copyable T>
    requires(not std::same_as<T, u8>)
  auto Chunk::write_instruction(const op::Code code, const T obj) -> void {
    debug_assert(op::byte_arg_count(code) == sizeof(T), "Incorrect number of arguments");

    *this << code;

    for (usize i = 0; i < sizeof(T); i++) {
      this->write_byte(std::bit_cast<const u8 *>(&obj)[i]);
    }

    // write_instruction(code);
    // write_instruction();
  }

  template<std::convertible_to<u8>... Bytes>
  auto Chunk::write_bytes(Bytes... bytes) -> void {
    std::ignore << (*this << ... << bytes);
  }

  template<std::copyable T>
  auto Chunk::get_bytes(const usize i) const -> T {
    u8 buffer[sizeof(T)];
    *buffer = bytes[i];

    std::memcpy(buffer, &bytes[i], sizeof(T));
    return std::bit_cast<T>(buffer);
  }
} // namespace goos::vm
