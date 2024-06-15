//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include <functional>
#include <utility>

#include "ICallable.hpp"
#include "IValue.hpp"

namespace goos::runtime {
  class BuiltinFunction final : public ICallable {
    std::function<Result<Any>(Environment &env, const Vec<Any> &args)> function;
    usize arity{};
    ArityType arity_type;

    explicit BuiltinFunction(decltype(function) function, usize arity, ArityType arity_type);

  public:
    static auto from(const usize arity, decltype(function) function) -> RcMut<BuiltinFunction> {
      return RcMut<BuiltinFunction>::from_owned_unchecked(
        new BuiltinFunction{std::move(function), arity, ArityType::Finite}
      );
    }

    static auto varargs(const decltype(function) &function) -> RcMut<BuiltinFunction> {
      return RcMut<BuiltinFunction>::from_owned_unchecked(
        new BuiltinFunction{function, 0, ArityType::Varargs}
      );
    }

    [[nodiscard]] auto call(Environment &env, const Vec<Any> &values) const -> Result<Any> override;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    [[nodiscard]] auto get_arity_type() const -> ArityType override;

    [[nodiscard]] auto get_arity() const -> usize override;

    [[nodiscard]] auto hash() const -> usize override;
  };
}
