#pragma once

#include <box.hpp>
#include <concepts>
#include <preamble.hpp>
#include "json/Array.hpp"
#include "json/Boolean.hpp"
#include "json/Number.hpp"
#include "json/Text.hpp"
#include "utils/str.hpp"

namespace goos::json {

  class Object;

  template<std::integral T>
  [[nodiscard]] auto to_json(const T val) -> Box<Number> {
    return crab::make_box<Number>(static_cast<f64>(val));
  }

  template<std::floating_point T>
  [[nodiscard]] auto to_json(const T val) -> Box<Number> {
    return crab::make_box<Number>(static_cast<f64>(val));
  }

  template<std::convertible_to<WideString> T>
  [[nodiscard]] auto to_json(const T& val) -> Box<Text> {
    return crab::make_box<Text>(static_cast<WideString>(val));
  }

  template<std::convertible_to<String> T>
  [[nodiscard]] auto to_json(const T& val) -> Box<Text> {
    return to_json(str::convert(static_cast<String>(val)));
  }

  [[nodiscard]] auto to_json(bool state) -> Box<Boolean>;

  [[nodiscard]] auto array() -> Box<Array>;

  [[nodiscard]] auto object() -> Box<Object>;

} // namespace goos::json
