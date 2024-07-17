//
// Created by bishan_ on 4/26/24.
//

#include "Eval.hpp"
#include <fmt/format.h>
#include <fmt/xchar.h>

#include "json/Object.hpp"

namespace goos::ast {
  auto Eval::get_expression() const -> const Expression & { return expression; }

  auto Eval::to_string() const -> WideString { return fmt::format(L"eval ({})", expression->to_string()); }

  auto Eval::clone() const -> Box<Statement> { return crab::make_box<Eval>(expression->clone_expr(), trace); }

  auto Eval::operator==(const Statement &statement) const -> bool {
    if (auto other{crab::ref::cast<Eval>(statement)}) {
      return *expression == *other.take_unchecked()->expression;
    }
    return false;
  }

  auto Eval::json() const -> Box<json::Value> {
    auto obj = crab::make_box<json::Object>();
    obj->put(L"type", L"eval");
    obj->put(L"expression", expression->json());
    return obj;
  }

  auto Eval::accept(IVisitor &visitor) const -> Result<unit, Box<crab::Error>> { return visitor.visit_eval(*this); }
} // namespace goos::ast
