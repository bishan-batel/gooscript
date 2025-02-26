//
// Created by bishan_ on 5/1/24.
//

#include "If.hpp"

#include <fmt/format.h>
#include <fmt/xchar.h>

#include "json/Object.hpp"

namespace goos::ast::expression {
  auto If::get_condition() const -> const Expression & { return condition; }

  auto If::get_then() const -> const Expression & { return then; }

  auto If::get_else_then() const -> const Expression & { return else_then; }

  auto If::to_string() const -> WideString {
    return fmt::format(
        L"if {} then {{ {} }} else {{ {} }}", condition->to_string(), then->to_string(), else_then->to_string());
  }

  auto If::clone_expr() const -> Box<Expression> {
    return crab::make_box<If>(condition->clone_expr(), then->clone_expr(), else_then->clone_expr(), trace);
  }

  auto If::operator==(const Statement &statement) const -> bool {
    if (auto other = crab::ref::cast<If>(statement)) {
      const auto &[condition, then, else_then, trace] = *other.get_unchecked();
      return *this->condition == *condition and *this->then == *then and *this->else_then == *else_then;
    }
    return false;
  }

  auto If::json() const -> Box<json::Value> {
    auto obj = crab::make_box<json::Object>();

    obj->put(L"type", L"if");
    obj->put(L"condition", condition->json());
    obj->put(L"then", then->json());
    obj->put(L"else", else_then->json());

    return obj;
  }

  auto If::accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> {
    return visitor.visit_if(*this);
  }
} // namespace goos::ast::expression
