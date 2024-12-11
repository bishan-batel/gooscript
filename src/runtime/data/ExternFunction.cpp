//
// Created by bishan_ on 6/14/24.
//

#include "ExternFunction.hpp"
#include "json/utils.hpp"

namespace goos::runtime {
  ExternFunction::ExternFunction(decltype(function) function, const usize arity, const ArityType arity_type)
      : function{std::move(function)}, arity{arity}, arity_type{arity_type} {}

  auto ExternFunction::from(const usize arity, decltype(function) function) -> RcMut<ExternFunction> {
    return RcMut<ExternFunction>::from_owned_unchecked(
        new ExternFunction{std::move(function), arity, ArityType::Finite});
  }

  auto ExternFunction::varargs(const decltype(function) &function) -> RcMut<ExternFunction> {
    return RcMut<ExternFunction>::from_owned_unchecked(new ExternFunction{function, 0, ArityType::Varargs});
  }

  auto ExternFunction::call(Intepreter &runtime, const Vec<Any> &values) const -> Result<Any, Box<err::Error>> {
    // TODO error checking
    return function(runtime, values);
  }

  auto ExternFunction::to_string() const -> WideString { return L"[builtin]"; }

  auto ExternFunction::get_type() const -> meta::VariantType { return meta::VariantType::FUNCTION; }

  auto ExternFunction::get_arity_type() const -> ArityType { return arity_type; }

  auto ExternFunction::get_arity() const -> usize { return arity; }

  auto ExternFunction::base_hash() const -> usize {
    // All builtin functions are unique.
    return utils::hash(this);
  }

  auto ExternFunction::clone() const -> Any {
    return RcMut<ExternFunction>::from_owned_unchecked(new ExternFunction{function, arity, arity_type});
  }
  auto ExternFunction::to_json() const -> Box<json::Value> { return json::to_json("[extern]"); }
} // namespace goos::runtime
