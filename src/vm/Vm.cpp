//
// Created by bishan_ on 6/21/24.
//

#include "Vm.hpp"

#include <cmath>
#include <result.hpp>
#include "vm/err/Error.hpp"

namespace goos::vm {
  Vm::Vm(Chunk chunk) : chunk{std::move(chunk)} {}

  auto Vm::print_stack() -> void {
    for (usize i = 0; i < stack.size(); i++) {
      // should never fail
      stack.peek(i).take_unchecked()->match(
          [&](const i64 x) -> void { log("{} => {}", i, x); },
          [&](const f64 x) { log("{} => {}", i, x); },
          [&](const bool x) -> void { log("{} => {}", i, x); },
          [&](const Nil) -> void { log("{} => nil", i); },
          [&](const unit) -> void { log("{} => unit", i); });
    }
  }

  auto Vm::push(const Value value) -> Result<unit> { return stack.push(value); }

  auto Vm::pop() -> Result<Value> { return stack.pop(); }

  auto Vm::next_byte() const -> u8 { return chunk.get_byte(registers.ip + 1); }

  auto Vm::run_instruction(const op::Code code) -> Result<unit> {
    switch (code) {
      case op::Code::RETURN: {
        break;
      }
      case op::Code::CONSTANT: {
        return push(chunk.get_constant(next_bytes<u16>()));
      }
      case op::Code::NIL: {
        return push(Nil::val);
      }
      case op::Code::UNIT: {
        return push(unit{});
      }
      case op::Code::TRUE: {
        return push(true);
      }
      case op::Code::FALSE: {
        return push(false);
      }

      case op::Code::SET: {
        return crab::fallible<Box<err::Error>>([&] { return stack.peek(next_bytes<u64>()); }, [&] { return pop(); })
            .map([&](std::tuple<RefMut<Value>, Value> values) {
              const auto [target, val] = values;
              *target = val;
              return unit{};
            });
      }

      case op::Code::GET: {
        return stack.peek(next_bytes<u64>()).and_then([&](Ref<Value> v) { return push(v); });
      }

      case op::Code::NOP: {
        return unit{};
      }

      case op::Code::POP: {
        return stack.pop().map([](auto) { return unit{}; });
      }
      case op::Code::SWAP: {
        return crab::fallible<Box<err::Error>>([&] { return stack.pop(); }, [&] { return stack.pop(); })
            .and_then([&](const std::tuple<Value, Value> vals) -> Result<unit> {
              const auto [first, second] = vals;

              if (Result<unit> r = stack.push(first); r.is_err()) {
                return r.take_err_unchecked();
              }

              return stack.push(second);
            });
      }
      case op::Code::JUMP: {
        const usize addr = next_bytes<u16>();
        registers.ip = addr - 1 - byte_arg_count(op::Code::JUMP_IF_FALSE);
        return unit{};
      }

      case op::Code::JUMP_IF_FALSE: {
        return pop_as<bool>().map([this](const bool condition) {
          if (not condition) {
            const usize addr = next_bytes<u16>();
            registers.ip = addr - 1 - byte_arg_count(op::Code::JUMP_IF_FALSE);
          }
          return unit{};
        });
      }

      case op::Code::NOT: {
        return pop_as<bool>().and_then([this](const bool cond) { return this->push(not cond); });
      }

      case op::Code::AND: {
        return crab::fallible<Box<err::Error>>([&] { return pop_as<bool>(); }, [&] { return pop_as<bool>(); })
            .and_then([&](auto val) {
              auto [first, second] = val;
              return this->push(first and second);
            });
      }
      case op::Code::OR: {
        return crab::fallible<Box<err::Error>>([this] { return pop_as<bool>(); }, [this] { return pop_as<bool>(); })
            .and_then([this](auto val) {
              auto [first, second] = val;
              return this->push(first or second);
            });
      }
      case op::Code::EQUALS: {
        return crab::fallible<Box<err::Error>>([this] { return pop(); }, [this] { return pop(); })
            .and_then([this](auto val) {
              auto [first, second] = val;
              return this->push(first == second);
            });
      }
      case op::Code::GREATER_THAN: {
#define bin_op(name, op)                                                                                               \
  crab::fallible<Box<err::Error>>([this] { return pop(); }, [this] { return pop(); })                                  \
      .and_then([this](auto tuple) -> Result<unit> {                                                                   \
        auto [second, first] = tuple;                                                                                  \
        return first.match(crab::cases{                                                                                \
            [&]<typename T, typename K>(const T a, const K b) { return push(Value{a op b}); },                         \
            [&]<typename T, typename K>                                                                                \
              requires(not requires(const T a, const K b) {                                                            \
                        { a op b } -> std::convertible_to<Value>;                                                      \
                      })                                                                                               \
            (const T, const K) -> Result<unit> {                                                                       \
              return vm::make_err<err::UnsupportedOperation>(                                                          \
                  name, std::vector{first.type_name(), second.type_name()});                                           \
            }},                                                                                                        \
            second);                                                                                                   \
      })

        return bin_op("GREATER_THAN", >);
      }

      case op::Code::LESS_THAN: {
        return bin_op("LESS_THAN", <);
      }
      case op::Code::GREATER_OR_EQUAL_THAN: {
        return bin_op("GREATER_OR_EQUAL_THAN", >=);
      }
      case op::Code::LESS_OR_EQUAL_THAN: {
        return bin_op("LESS_OR_EQUAL_THAN", <=);
      }
      case op::Code::BIT_AND: {
        return bin_op("BIT_AND", &);
      }
      case op::Code::BIT_OR: {
        return bin_op("BIT_OR", |);
      }
      case op::Code::BIT_XOR: {
        return bin_op("BIT_XOR", ^);
      }
      case op::Code::SHIFT_LEFT: {
        return bin_op("SHIFT_LEFT", <<);
      }
      case op::Code::SHIFT_RIGHT: {
        return bin_op("SHIFT_RIGHT", >>);
      }
      case op::Code::ADD: {
        return bin_op("ADD", +);
      }
      case op::Code::SUBTRACT: {
        return bin_op("SUBTRACT", -);
      }
      case op::Code::MULIPLY: {
        return bin_op("MULIPLY", *);
      }
      case op::Code::DIVIDE: {
        return bin_op("DIVIDE", /);
      }
      case op::Code::MODULO: {
        return crab::fallible<Box<err::Error>>([this] { return pop(); }, [this] { return pop(); })
            .and_then([this](auto tuple) -> Result<unit> {
              auto [first, second] = tuple;
              return first.match(
                  crab::cases{
                      [&](const f64 a, const f64 b) { return push(std::fmod(a, b)); },
                      [&](const i64 a, const f64 b) { return push(std::fmod(a, b)); },
                      [&](const f64 a, const i64 b) { return push(std::fmod(a, b)); },

                      [&]<typename T, typename K>(T, K) -> Result<unit> {
                        return vm::make_err<err::UnsupportedOperation>(
                            "MODULO", std::vector{Value::type_name_of_s<T>, Value::type_name_of_s<K>});
                      }},
                  second);
            });
      }
#undef bin_op
      case op::Code::NEGATE: {
        auto popped = pop();
        if (popped.is_err())
          return popped.take_err_unchecked();

        return popped.take_unchecked().match(
            [this](const auto n) -> Result<unit> { return this->push(-n); },
            []<typename T>
              requires(not requires(const T n) {
                        { -n } -> std::convertible_to<Value>;
                      })
            (const T) -> Result<unit> {
              return make_err<err::UnsupportedOperation>("NEGATE", std::vector{Value::type_name_of_s<T>});
            });
      }

      case op::Code::PRINT: {
        print_stack();
        break;
      }

      case op::Code::DUP:
        return stack.peek().and_then([this](const Value val) { return this->push(val); });
    }

    return unit::val;
  }

  auto Vm::run(const usize index) -> Result<Value> {
    registers.ip = index;

    while (registers.ip < chunk.byte_count()) {
      const op::Code code = op::from_byte(chunk.get_byte(registers.ip));

      // fmt::print("{:#04x}> ", registers.ip);
      // chunk.dissassemble_instruction(registers.ip);

      if (auto err = run_instruction(code); err.is_err()) {
        return err.take_err_unchecked();
      }
      registers.ip += 1 + byte_arg_count(code);

      // using namespace std::chrono;
      // std::this_thread::sleep_for(50ms);
    }

    // if (stack.empty()) return Value{unit::val};

    return pop();
  }
} // namespace goos::vm
