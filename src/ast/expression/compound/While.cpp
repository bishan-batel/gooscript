//
// Created by bishan_ on 5/2/24.
//

#include "While.hpp"
#include <fmt/format.h>
#include <fmt/xchar.h>

namespace goos::ast::expression {
  While::While(Box<Expression> condition, Box<Expression> body)
    : condition{std::move(condition)}, body{std::move(body)} {}

  const Expression& While::get_condition() const { return condition; }

  const Expression& While::get_body() const { return body; }

  WideString While::to_string() const {
    return fmt::format(L"while {} {{ {} }}", condition->to_string(), body->to_string());
  }

  Box<Expression> While::clone_expr() const {
    return crab::make_box<While>(condition->clone_expr(), body->clone_expr());
  }
} // namespace goos::ast::expression
