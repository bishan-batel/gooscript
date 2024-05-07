//
// Created by bishan_ on 5/2/24.
//

#include "While.hpp"

#include <fmt/format.h>
#include <fmt/xchar.h>

namespace goos::ast::expression {
  While::While(Box<Expression> condition, Box<Expression> body)
    : condition{std::move(condition)}, body{std::move(body)} {}

  auto While::get_condition() const -> const Expression& { return condition; }

  auto While::get_body() const -> const Expression& { return body; }

  auto While::to_string() const -> WideString {
    return fmt::format(L"while {} {{ {} }}", condition->to_string(), body->to_string());
  }

  auto While::clone_expr() const -> Box<Expression> {
    return crab::make_box<While>(condition->clone_expr(), body->clone_expr());
  }

  auto While::operator==(const Statement &statement) const -> bool {
    if (auto other = crab::ref::cast<While>(statement)) {
      const auto &[condition, body] = *other.get_unchecked();
      return *this->condition == *condition and *this->body == *body;
    }
    return false;
  }
} // namespace goos::ast::expression
