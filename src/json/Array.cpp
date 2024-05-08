//
// Created by bishan_ on 5/8/24.
//

#include "Array.hpp"

namespace goos::json {
  Array::Array() = default;

  auto Array::push(Box<Value> value) -> void {
    values.push_back(std::move(value));
  }

  auto Array::length() const -> usize {
    return values.size();
  }

  auto Array::write(std::wostream &os) const -> void {
    os << '[';

    usize count = 0;
    for (const auto &value: values) {
      value->write(os);

      if (++count < values.size()) os << ',';
    }
    os << ']';
  }

  auto Array::clone() const -> Box<Value> {
    auto arr = crab::make_box<Array>();

    for (const auto &v: values) {
      arr->push(v->clone());
    }

    return arr;
  }
}
