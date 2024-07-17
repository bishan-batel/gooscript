//
// Created by bishan_ on 4/26/24.
//

#include "Unit.hpp"

#include "json/Object.hpp"
#include "json/Text.hpp"

namespace goos::ast::expression {
  auto Unit::to_string() const -> WideString {
    return L"unit";
  }

  auto Unit::clone_expr() const -> Box<Expression> { return crab::make_box<Unit>(trace); }

  auto Unit::operator==(const Statement &statement) const -> bool {
    return crab::ref::cast<Unit>(statement).is_some();
  }

  auto Unit::json() const -> Box<json::Value> {
    auto obj = crab::make_box<json::Object>();
    obj->put(L"type", crab::make_box<json::Text>(L"unit"));
    return obj;
  }

  auto Unit::accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> {
    return visitor.visit_unit(*this);
  }
}
