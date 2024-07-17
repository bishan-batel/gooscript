//
// Created by bishan_ on 6/27/24.
//

#include "Builder.hpp"
#include <utility>
#include "vm/Instruction.hpp"

namespace goos::codegen {
  using namespace vm;

  Builder::Builder(WideString name) : name{std::move(name)} {}

  auto Builder::write_push_constant(const vm::Value value) -> Value {
    const u16 index = lookup_constant(value);
    ;

    instructions.emplace_back(op::Code::CONSTANT, index);

    stack_tracker += op::stack_influence(vm::op::Code::CONSTANT);

    return unit{};
  }

  auto Builder::make_unlinked_label() -> Label { return Label{num_labels++}; }

  auto Builder::define_label(const Label label) -> void { label_lookup[label] = instructions.size(); }

  auto Builder::write_jmp(const Label label) -> Value {
    instructions.emplace_back(op::Code::JUMP, label);
    stack_tracker += op::stack_influence(vm::op::Code::JUMP);
    return unit{};
  }

  auto Builder::write_jmp_if_false(const Label label) -> Value {
    instructions.emplace_back(op::Code::JUMP_IF_FALSE, label);
    stack_tracker += op::stack_influence(vm::op::Code::JUMP_IF_FALSE);
    return unit{};
  }

  auto Builder::instruction_to_bytecode_index(const usize instruction_index) const -> usize {
    debug_assert(instruction_index <= instructions.size(), "Invalid instruction index");

    usize index = 0;

    for (usize i = 0; i < instruction_index; i++) {
      index += 1 + byte_arg_count(instructions[i].code);
    }

    return index;
  }

  auto Builder::finalize_chunk() -> Chunk {
    Chunk chunk{std::move(name)};

    for (const auto &constant: constants) {
      chunk.define_constant(constant);
    }

    for (const auto &[code, var]: instructions) {
      std::visit(
          crab::cases{
              [&](const u16 data) { chunk.write_instruction(code, data); },

              [&](const Label label) {
                chunk.write_instruction(code, instruction_to_bytecode_index(label_lookup.at(label)));
              },
              [&](unit) { chunk.write_instruction(code); },
          },
          var);
    }

    return chunk;
  }

  auto Builder::lookup_constant(const vm::Value value) -> u16 {
    u16 i = 0;

    for (const auto &v: constants) {
      if (v == value) {
        return i;
      }
      i++;
    }

    constants.push_back(value);

    return i;
  }

  auto Builder::push_env() -> void { current_enviornment = Enviornment::enclose(current_enviornment); }

  auto Builder::pop_env() -> RcMut<Enviornment> {
    return std::exchange(current_enviornment, current_enviornment->get_enclosing().get_unchecked());
  }
} // namespace goos::codegen

auto std::hash<goos::codegen::Label>::operator()(const goos::codegen::Label &label) const noexcept -> usize {
  return label.id;
}
