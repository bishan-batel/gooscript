//
// Created by bishan_ on 6/27/24.
//

#pragma once
#include <crab/debug.hpp>
#include "Enviornment.hpp"
#include "Value.hpp"
#include "ast/Statement.hpp"
#include "vm/Chunk.hpp"
#include "vm/Instruction.hpp"

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
      std::variant<u64, u16, Label, unit> args;
      ast::TokenTrace trace;

      Instruction(const vm::op::Code code, const decltype(args) args, ast::TokenTrace trace)
          : code{code}, args{args}, trace{trace} {}
    };

    // vm::Chunk chunk;
    Vec<Instruction> instructions{};

    Vec<vm::Value> constants{};

    // Label to Index
    Dictionary<Label, usize> label_lookup{};

    Dictionary<WideString, usize> variable_lookup{};

    u64 stack_tracker = 0;

    u64 num_labels = 0;

    RcMut<Enviornment> current_enviornment{Enviornment::standard_enviornment()};

    WideString name;

  public:
    explicit Builder(WideString name);

    auto make_unlinked_label() -> Label;

    auto define_label(Label label) -> void;

    auto write_push_constant(vm::Value value, ast::TokenTrace trace) -> Value;

    auto write_jmp(Label label, ast::TokenTrace trace) -> Value;

    auto write_jmp_if_false(Label label, ast::TokenTrace trace) -> Value;

    template<vm::op::Code Code>
    auto write(ast::TokenTrace trace) -> Value {
      static_assert(vm::op::byte_arg_count(Code) == 0, "Written bytecode does not have 0 arguments");
      instructions.emplace_back(Code, unit{}, trace);
      stack_tracker -= vm::op::stack_influence(Code);
      return unit{};
    }

    auto write_get(u64 offset, ast::TokenTrace trace) -> Value {
      instructions.emplace_back(vm::op::Code::GET, offset, trace);
      stack_tracker -= vm::op::stack_influence(vm::op::Code::GET);
      return unit{};
    }

    auto write_set(u64 offset, ast::TokenTrace trace) -> Value {
      instructions.emplace_back(vm::op::Code::SET, offset, trace);
      stack_tracker -= vm::op::stack_influence(vm::op::Code::SET);
      return unit{};
    }

    auto instruction_to_bytecode_index(usize instruction_index) const -> usize;

    [[nodiscard]] auto finalize_chunk() -> vm::Chunk;

    [[nodiscard]] auto current_env() const -> RcMut<Enviornment> { return current_enviornment; }

    auto push_env() -> void;

    auto pop_env() -> RcMut<Enviornment>;

    [[nodiscard]] auto get_stack_tracker() const { return stack_tracker; }

    auto reset_stack_tracker() { stack_tracker = 0; }

  private:
    auto lookup_constant(vm::Value value) -> u16;
  };
} // namespace goos::codegen
