//
// Created by bishan_ on 5/3/24.
//

#include "Array.hpp"

#include <sstream>
#include <fmt/format.h>
#include <fmt/xchar.h>

namespace goos::ast::expression {
  Array::Array(Vec<Box<Expression>> values) : values{std::move(values)} {}

  auto Array::to_string() const -> WideString {
    WideStringStream stream{};
    stream << '[';
    for (const auto &value: values) {
      stream << fmt::format(L"{}, ", value->to_string());
    }
    stream << ']';
    return stream.str();
  }

  auto Array::clone_expr() const -> Box<Expression> {
    Vec<Box<Expression>> cloned{};

    for (const auto &value: values) {
      cloned.push_back(value->clone_expr());
    }

    return crab::make_box<Array>(std::move(cloned));
  }

  auto Array::operator==(const Statement &statement) const -> bool {
    auto other_opt{crab::ref::cast<Array>(statement)};

    if (other_opt.is_none()) return false;

    const Array &array = other_opt.take_unchecked();

    if (array.values.size() != values.size()) return false;

    for (usize i = 0; i < values.size(); i++) {
      if (*values[i] != *array.values[i]) return false;
    }
    return true;
  }
}
