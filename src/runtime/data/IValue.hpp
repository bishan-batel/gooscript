//
// Created by bishan_ on 5/15/24.
//

#pragma once
#include <memory>
#include <rc.hpp>

#include "preamble.hpp"
#include "meta/VariantType.hpp"

namespace goos::runtime {
  class IValue;

  class IValue {
  public:
    IValue() = default;

    IValue(IValue &) = delete;

    virtual ~IValue() = default;

    auto operator=(IValue &) -> IValue& = delete;

    IValue(IValue &&) = default;

    auto operator=(IValue &&) -> IValue& = default;

    template<typename T>
    auto coerce() -> Option<RefMut<T>>;

    template<typename T>
    auto coerce_unchecked() -> T&;

    [[nodiscard]] virtual auto to_string() const -> WideString = 0;

    [[nodiscard]] virtual auto get_type() const -> meta::VariantType = 0;

    [[nodiscard]] virtual auto is_truthy() const -> bool { return false; }
  };

  template<typename T>
  auto IValue::coerce() -> Option<RefMut<T>> {
    return crab::ref::cast<T>(*this);
  }

  template<typename T>
  auto IValue::coerce_unchecked() -> T& {
    return coerce<T>().take_unchecked();
  }
}
