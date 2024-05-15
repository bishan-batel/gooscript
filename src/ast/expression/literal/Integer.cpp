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

  auto Integer::get_number() const -> i64 { return number; }

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
    return crab::make_box<json::Number>(static_cast<f32>(number));
  }

  auto Integer::accept_expr(IVisitor &visitor) const -> std::shared_ptr<runtime::Value> {
    return visitor.visit_integer(*this);
  }
}
