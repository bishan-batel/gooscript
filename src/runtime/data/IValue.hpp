//
// Created by bishan_ on 5/15/24.
//

#pragma once
#include <memory>
#include <rc.hpp>

#include "preamble.hpp"
#include "meta/VariantType.hpp"
#include "utils/hash.hpp"

namespace goos::runtime {
  #if __JETBRAINS_IDE__
  [[jetbrains::pass_by_value]]
  #endif
  using Any = RcMut<struct IValue>;

  struct IValue {
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

    [[nodiscard]] virtual auto clone() const -> Any = 0;

    [[nodiscard]] virtual auto to_string() const -> WideString = 0;

    [[nodiscard]] virtual auto get_type() const -> meta::VariantType = 0;

    [[nodiscard]] virtual auto is_truthy() const -> bool { return false; }

    [[nodiscard]] virtual auto base_hash() const -> usize = 0;

    [[nodiscard]] auto hash() const -> usize {
      return utils::combine_hash(base_hash(), static_cast<usize>(get_type()));
    }
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

template<>
struct std::hash<goos::runtime::IValue> {
  auto operator()(const goos::runtime::IValue &value) const noexcept -> usize {
    return value.base_hash();
  }
};
