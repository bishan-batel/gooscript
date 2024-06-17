//
// Created by bishan_ on 6/16/24.
//

#include "LValue.hpp"
#include <fmt/xchar.h>

namespace goos::runtime {
  LValue::LValue(const RefMut<Any> inner) : inner{inner} {}

  auto LValue::wrap(Any &value) -> RcMut<LValue> {
    return wrap(RefMut{value});
  }

  auto LValue::wrap(const RefMut<Any> value) -> RcMut<LValue> {
    return RcMut<LValue>::from_owned_unchecked(new LValue{value});
  }

  auto LValue::set(Any value) const -> void {
    *inner = std::move(value);
  }

  auto LValue::get() const -> Any {
    return *inner;
  }

  auto LValue::clone() const -> Any {
    return wrap(inner);
  }

  auto LValue::to_string() const -> WideString {
    return fmt::format(L"[&]{}", inner->get().to_string());
  }

  auto LValue::get_type() const -> meta::VariantType {
    return meta::VariantType::UNIT;
  }

  auto LValue::base_hash() const -> utils::hash_code {
    return utils::hash(inner.as_ptr());
  }
} // goos
