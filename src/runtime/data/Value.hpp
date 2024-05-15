//
// Created by bishan_ on 5/15/24.
//

#pragma once
#include <memory>
#include <rc.hpp>

#include "preamble.hpp"
#include "meta/VariantType.hpp"

namespace goos::runtime {
  class Value;
  // TODO when crab supports mutable Rc, switch to that over std::shared_ptr.
  template<typename T>
  using Rc = std::shared_ptr<T>;

  using Any = Rc<Value>;

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

    virtual auto to_string() const -> WideString = 0;

    virtual auto get_type() const -> meta::VariantType = 0;
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
