//
// Created by bishan_ on 5/16/24.
//

#include "Boolean.hpp"
#include "json/utils.hpp"

namespace goos::runtime {
  // auto Boolean::set(const bool v) -> void {
  //   state = v;
  // }

  Boolean::Boolean(const bool state) : state{state} {}

  auto Boolean::get() const -> bool { return state; }

  auto Boolean::to_string() const -> WideString { return state ? L"true" : L"false"; }

  auto Boolean::get_type() const -> meta::VariantType { return meta::VariantType::BOOLEAN; }

  auto Boolean::is_truthy() const -> bool { return state; }

  auto Boolean::base_hash() const -> usize { return utils::hash(state); }

  auto Boolean::clone() const -> Any { return crab::make_rc_mut<Boolean>(state); }

  auto Boolean::to_json() const -> Box<json::Value> { return json::to_json(state); }
} // namespace goos::runtime
