//
// Created by bishan_ on 6/14/24.
//

#include "NotCallable.hpp"

#include "runtime/data/Value.hpp"
#include "utils/str.hpp"

namespace goos {
  runtime::err::NotCallable::NotCallable(Any value): value{std::move(value)} {}

  auto runtime::err::NotCallable::get_value() const -> Any {
    return value;
  }

  String runtime::err::NotCallable::what() const {
    return fmt::format(
      "'{}' (type: {}) is not callable.",
      str::convert(value->to_string()),
      static_cast<usize>(value->get_type())
    );
  }
} // goos
