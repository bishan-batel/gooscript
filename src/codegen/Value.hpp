//
// Created by bishan_ on 6/28/24.
//

#pragma once
#include "vm/Value.hpp"

namespace goos::codegen {
  class Constant final {
  public:
    explicit Constant(const vm::Value val)
      : val{val} {}

  private:
    vm::Value val;
  };

  class Value final {
  public:
    using Variant = std::variant<unit, Constant>;

  private:
    Variant variant;

  public:
    // ReSharper disable once CppNonExplicitConvertingConstructor
    Value(const Variant &variant) : variant{variant} {}

    // ReSharper disable once CppNonExplicitConvertingConstructor
    template<std::convertible_to<Variant> T>
    Value(const T &variant) : Value{Variant{variant}} {}

    [[nodiscard]] auto type_hash() const -> usize;

    // [[nodiscard]] auto get_id() const -> usize;

    [[nodiscard]] auto get_variant() const -> const Variant&;
  };
} // goos
