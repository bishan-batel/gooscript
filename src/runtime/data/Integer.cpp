//
// Created by bishan_ on 5/15/24.
//

#include "Integer.hpp"

namespace goos::runtime {
  Integer::Integer(const i64 value) : value{value} {}

  auto Integer::to_string() const -> WideString {
    return std::to_wstring(value);
  }

  auto Integer::get_type() const -> meta::VariantType {
    return meta::VariantType::INTEGER;
  }

  auto Integer::set(const i64 v) -> void {
    value = v;
  }

  auto Integer::get() const -> i64 {
    return value;
  }

  auto Integer::set_number(const f64 v) -> void {
    value = static_cast<i64>(v);
  }

  auto Integer::get_number() const -> f64 {
    return value;
  }

  auto Integer::is_integral() const -> bool {
    return true;
  }
}
