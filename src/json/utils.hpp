#pragma once

#include <box.hpp>
#include <concepts>
#include <preamble.hpp>
#include "json/Array.hpp"
#include "json/Boolean.hpp"
#include "json/Number.hpp"
#include "json/Object.hpp"
#include "json/Text.hpp"
#include "utils/str.hpp"

namespace goos::json {

  template<std::integral T>
  auto to_json(const T val) -> Box<Number> {
    return crab::make_box<Number>(static_cast<f64>(val));
  }

  template<std::floating_point T>
  auto to_json(const T val) -> Box<Number> {
    return crab::make_box<Number>(static_cast<f64>(val));
  }

  template<std::convertible_to<WideString> T>
  auto to_json(const T &val) -> Box<Text> {
    return crab::make_box<Text>(static_cast<WideString>(val));
  }

  template<std::convertible_to<String> T>
  auto to_json(const T &val) -> Box<Text> {
    return to_json(str::convert(static_cast<String>(val)));
  }

  inline auto to_json(const bool state) -> Box<Boolean> { return crab::make_box<Boolean>(state); }

  inline auto array() -> Box<Array> { return crab::make_box<Array>(); }

  inline auto object() -> Box<Object> { return crab::make_box<Object>(); }

} // namespace goos::json
