//
// Created by bishan_ on 6/17/24.
//

#include "ArrayIndex.hpp"

#include "fmt/xchar.h"
#include "json/Object.hpp"

namespace goos::ast::expression {

  auto ArrayIndex::get_object() const -> const Expression & { return object; }

  auto ArrayIndex::get_index() const -> const Expression & { return index; }

  auto ArrayIndex::operator==(const Statement &statement) const -> bool {
    if (auto casted = statement.try_as<ArrayIndex>()) {
      const ArrayIndex &other = *casted.take_unchecked();
      return *object == *other.object and *index == *other.index;
    }
    return false;
  }

  auto ArrayIndex::to_string() const -> WideString {
    return fmt::format(L"{}[{}]", object->to_string(), index->to_string());
  }

  auto ArrayIndex::json() const -> Box<json::Value> {
    auto json = crab::make_box<json::Object>();

    json->put(L"object", object->json());
    json->put(L"index", index->json());

    return json;
  }

  auto ArrayIndex::clone_expr() const -> Box<Expression> {
    return crab::make_box<ArrayIndex>(object->clone_expr(), index->clone_expr(), closing_bracket_trace);
  }

  auto ArrayIndex::accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> {
    return visitor.visit_array_index(*this);
  }
  auto ArrayIndex::token_trace() const -> TokenTrace {
    return TokenTrace::merge_fold(object->token_trace(), index->token_trace(), closing_bracket_trace);
  }
} // namespace goos::ast::expression
