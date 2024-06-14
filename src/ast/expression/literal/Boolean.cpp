//
// Created by bishan_ on 4/26/24.
//

#include "Boolean.hpp"

#include <fmt/format.h>
#include <fmt/xchar.h>

#include "json/Text.hpp"

namespace goos::ast::expression {
  auto Boolean::get_state() const -> bool {
    return state;
  }

  auto Boolean::to_string() const -> WideString {
    return fmt::format(L"{}", state);
  }

  auto Boolean::clone_expr() const -> Box<Expression> {
    return crab::make_box<Boolean>(state);
  }

  auto Boolean::operator==(const Statement &statement) const -> bool {
    if (auto other{crab::ref::cast<Boolean>(statement)}) {
      return other.take_unchecked()->state == state;
    }
    return false;
  }

  auto Boolean::json() const -> Box<json::Value> {
    // TODO boolean literal
    return crab::make_box<json::Text>(state ? L"true" : L"false");
  }

  auto Boolean::accept_expr(IVisitor &visitor) const -> runtime::Result<runtime::Any> {
    return visitor.visit_boolean(*this);
  }
}
