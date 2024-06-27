//
// Created by bishan_ on 6/21/24.
//

#pragma once
#include <stack>
#include <variant>
#include <fmt/format.h>
#include <fmt/color.h>

#include "Chunk.hpp"
#include "Stack.hpp"
#include "err/Error.hpp"
#include "Value.hpp"
#include "token/Token.hpp"

namespace goos::vm {
  class Vm {
    struct Local {
      Option<Rc<token::Token>> name;
      Value value;
      usize depth;
    };

    Chunk chunk;

    Stack stack;
    std::vector<Local> locals;

    struct {
      usize ip = 0;
    } registers{};

  public:
    explicit Vm(Chunk chunk);

    auto print_stack() -> void;

    [[nodiscard]] auto run(usize index = 0) -> Result<Value>;

  private:
    [[nodiscard]] auto push(Value value) -> Result<unit>;

    [[nodiscard]] auto pop() -> Result<Value>;

    template<typename T>
    [[nodiscard]] auto pop_as() -> Result<T>;

    [[nodiscard]] auto next_byte() const -> u8;

    template<std::copyable T>
    [[nodiscard]] auto next_bytes() const -> T;

    [[nodiscard]] auto run_instruction(op::Code code) -> Result<unit>;

    template<typename S, typename... T>
    auto log(S fmt, T &&... args);
  };

  template<typename T>
  auto Vm::pop_as() -> Result<T> {
    return pop().and_then([](const Value val) { return val.as<T>(); });
  }

  template<std::copyable T>
  auto Vm::next_bytes() const -> T {
    return chunk.get_bytes<T>(registers.ip + 1);
  }

  template<typename S, typename... T>
  auto Vm::log(S fmt, T &&... args) {
    constexpr fmt::text_style comment_style = fg(fmt::color::light_gray) | fmt::emphasis::italic;

    print(fg(fmt::color::gray), "    > {}", styled("// ", comment_style));
    fmt::print(
      fg(fmt::color::light_gray) | fmt::emphasis::italic,
      fmt,
      std::forward<T>(args)...
    );
    fmt::println("");
  }
} // goos
