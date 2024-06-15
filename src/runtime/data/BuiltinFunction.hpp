//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include <functional>

#include "Callable.hpp"
#include "Value.hpp"

namespace goos::runtime {
  class BuiltinFunction final : public Callable {
    std::function<Result<Any>(Vec<Any>)> function;
    usize arity{};
    ArityType arity_type;

    explicit BuiltinFunction(std::function<Result<Any>(Vec<Any>)> function, usize arity, ArityType arity_type);

  public:
    static auto from(usize arity, decltype(function) function) -> RcMut<BuiltinFunction> {
      return RcMut<BuiltinFunction>::from_owned_unchecked(
        new BuiltinFunction{std::move(function), arity, ArityType::Finite}
      );
    }

    static auto varargs(decltype(function) function) -> RcMut<BuiltinFunction> {
      return RcMut<BuiltinFunction>::from_owned_unchecked(
        new BuiltinFunction{std::move(function), 0, ArityType::Varargs}
      );
    }

    [[nodiscard]] auto call(const Vec<Any> &values) const -> Result<Any> override;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    [[nodiscard]] auto get_arity_type() const -> ArityType override;

    [[nodiscard]] auto get_arity() const -> usize override;
  };
}
