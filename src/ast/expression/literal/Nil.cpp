//
// Created by bishan_ on 4/27/24.
//

#include "Nil.hpp"

#include "json/Object.hpp"
#include "json/Text.hpp"

namespace goos::ast::expression {
  auto Nil::to_string() const -> WideString {
    return L"nil";
  }

  auto Nil::clone_expr() const -> Box<Expression> { return crab::make_box<Nil>(); }

  auto Nil::operator==(const Statement &statement) const -> bool {
    return crab::ref::cast<Nil>(statement).is_some();
  }

  auto Nil::json() const -> Box<json::Value> {
    auto obj = crab::make_box<json::Object>();
    obj->put(L"type", crab::make_box<json::Text>(L"nil"));
    return obj;
  }

  auto Nil::accept_expr(IVisitor &visitor) const -> RcMut<runtime::Value> {
    return visitor.visit_nil(*this);
  }
} // goos
