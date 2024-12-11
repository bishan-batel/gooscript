
//
// Created by bishan_ on 5/8/24.
//

#include "Boolean.hpp"

namespace goos::json {
  Boolean::Boolean(const bool state) : state{state} {}

  auto Boolean::set(const bool num) -> void { state = num; }

  auto Boolean::get() const -> bool { return state; }

  auto Boolean::write(std::wostream &os) const -> void { os << state; }

  auto Boolean::clone() const -> Box<Value> { return crab::make_box<Boolean>(state); }
} // namespace goos::json
