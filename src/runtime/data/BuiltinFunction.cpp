//
// Created by bishan_ on 6/14/24.
//

#include "BuiltinFunction.hpp"

namespace goos::runtime {
  BuiltinFunction::BuiltinFunction(
    std::function<Result<Any>(Vec<Any>)> function,
    const usize arity,
    const ArityType arity_type
  )
    : function{std::move(function)},
      arity{arity},
      arity_type{arity_type} {}

  auto BuiltinFunction::call(const Vec<Any> &values) const -> Result<Any> {
    // TODO error checking
    return function(values);
  }

  auto BuiltinFunction::to_string() const -> WideString {
    return L"[builtin]";
  }

  auto BuiltinFunction::get_type() const -> meta::VariantType {
    return meta::VariantType::FUNCTION;
  }

  auto BuiltinFunction::get_arity_type() const -> ArityType {
    return arity_type;
  }

  auto BuiltinFunction::get_arity() const -> usize {
    return arity;
  }
}
