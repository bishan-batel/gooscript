//
// Created by bishan_ on 5/15/24.
//

#pragma once
#include <memory>
#include <rc.hpp>

#include "preamble.hpp"
#include "meta/VariantType.hpp"
#include "runtime/Environment.hpp"

namespace goos::runtime {
  class Value;

  class Value {
  public:
    Value() = default;

    Value(Value &) = delete;

    virtual ~Value() = default;

    auto operator=(Value &) -> Value& = delete;

    Value(Value &&) = default;

    auto operator=(Value &&) -> Value& = default;

    template<typename T>
    auto coerce() -> Option<RefMut<T>>;

    template<typename T>
    auto coerce_unchecked() -> T&;

    [[nodiscard]] virtual auto to_string() const -> WideString = 0;

    [[nodiscard]] virtual auto get_type() const -> meta::VariantType = 0;

    [[nodiscard]] virtual auto is_truthy() const -> bool { return false; }
  };

  template<typename T>
  auto Value::coerce() -> Option<RefMut<T>> {
    return crab::ref::cast<T>(*this);
  }

  template<typename T>
  auto Value::coerce_unchecked() -> T& {
    return coerce<T>().take_unchecked();
  }
}
