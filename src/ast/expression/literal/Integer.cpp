//
// Created by bishan_ on 4/26/24.
//

#include "Integer.hpp"

#include <fmt/format.h>
#include <fmt/xchar.h>

#include "json/Number.hpp"
#include "json/Object.hpp"

namespace goos::ast::expression {
  Integer::Integer(const i64 number): number{number} {}

  auto Integer::to_string() const -> WideString {
    return fmt::format(L"{}", number);
  }

  auto Integer::clone_expr() const -> Box<Expression> { return crab::make_box<Integer>(number); }

  auto Integer::operator==(const Statement &statement) const -> bool {
    if (auto other = crab::ref::cast<Integer>(statement)) {
      return other.take_unchecked()->number == number;
    }
    return false;
  }

  auto Integer::json() const -> Box<json::Value> {
    return crab::make_box<json::Number>(number);
  }
}
