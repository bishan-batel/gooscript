//
// Created by bishan_ on 6/24/24.
//

#include "Stack.hpp"

namespace goos {
  vm::Stack::Stack() = default;

  auto vm::Stack::peek(const usize offset) -> Result<RefMut<Value>> {
    const usize index = top - offset - 1;

    // TODO OutOfBounds Error
    debug_assert(index < top, "Invalid Index");
    return RefMut{buffer[index]};
  }

  auto vm::Stack::peek(const usize offset) const -> Result<Ref<Value>> {
    const usize index = top - offset - 1;

    // TODO OutOfBounds Error
    debug_assert(index < top, "Invalid Index");
    return Ref{buffer[index]};
  }

  auto vm::Stack::pop() -> Result<Value> {
    // return peek().map(
    //   [this](const Value) {
    //     top--;
    //
    //     const Value val = buffer[top];
    //     buffer[top] = unit::val;
    //     return val;
    //   }
    // );
    top--;

    const Value val = buffer[top];
    buffer[top] = unit::val;
    return val;
  }

  auto vm::Stack::size() const -> usize { return top; }

  auto vm::Stack::empty() const -> usize { return size() == 0; }

  auto vm::Stack::push(const Value value) -> Result<unit> {
    if (top >= buffer.size()) {
      top++;
      buffer.push_back(value);
      return unit{};
    }

    buffer[top++] = value;
    return unit{};
  }
} // namespace goos
