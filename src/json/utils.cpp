#include "utils.hpp"
#include "json/Object.hpp"

auto goos::json::object() -> Box<Object> { return crab::make_box<Object>(); }

auto goos::json::to_json(const bool state) -> Box<Boolean> {
  return crab::make_box<Boolean>(state);
}

auto goos::json::array() -> Box<Array> { return crab::make_box<Array>(); }
