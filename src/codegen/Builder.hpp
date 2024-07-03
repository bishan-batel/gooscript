//
// Created by bishan_ on 6/27/24.
//

#pragma once
#include "Value.hpp"
#include "utils/hash.hpp"
#include "vm/Chunk.hpp"

namespace goos::codegen {
  struct Label;
}

template<>
struct std::hash<goos::codegen::Label> {
  auto operator()(const goos::codegen::Label &label) const noexcept -> usize;
};

namespace goos::codegen {
  struct Label final {
    u64 id;

    explicit Label(const u64 id) : id{id} {}

    auto operator==(const Label &l) const -> bool { return id == l.id; }
  };

  class Builder final {
    struct Instruction {
      vm::op::Code code;
      std::variant<u16, Label, unit> args;

      Instruction(const vm::op::Code code, const decltype(args) args) : code{code}, args{args} {}
    };

    // vm::Chunk chunk;
    Vec<Instruction> instructions{};

    Vec<vm::Value> constants{};

    // Label to Index
    Dictionary<Label, usize> label_lookup{};

    u64 num_labels = 0;
    WideString name;

  public:
    explicit Builder(WideString name);

    auto write_push_constant(vm::Value value) -> Value;

    auto make_unlinked_label() -> Label;

    auto define_label(Label label) -> void;

    auto write_jmp(Label label) -> Value;

    auto write_jmp_if_false(Label lable) -> Value;

    template<vm::op::Code Code>
    auto write() -> Value {
      instructions.emplace_back(Code, unit{});
      return unit{};
    }

    auto instruction_to_bytecode_index(usize instruction_index) const -> usize;

    [[nodiscard]] auto finalize_chunk() -> vm::Chunk;

  private:
    auto lookup_constant(vm::Value value) -> u16;
  };
} // goos
