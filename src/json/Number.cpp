//
// Created by bishan_ on 5/8/24.
//

#include "Number.hpp"

namespace goos::json {
  Number::Number(const f64 number) : number{number} {}

  auto Number::set(const f64 num) -> void {
    number = num;
  }

  auto Number::get() const -> f64 {
    return number;
  }

  auto Number::write(std::wostream &os) const -> void {
    os << number;
  }

  auto Number::clone() const -> Box<Value> {
    return crab::make_box<Number>(number);
  }
}
