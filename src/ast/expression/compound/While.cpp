//
// Created by bishan_ on 5/2/24.
//

#include "While.hpp"
#include <fmt/format.h>

namespace goos::ast::expression {
  While::While(Box<Expression> condition, Box<Expression> body)
    : condition{std::move(condition)}, body{std::move(body)} {}

  const Expression& While::get_condition() const { return condition; }

  const Expression& While::get_body() const { return body; }

  String While::to_string() const {
    return fmt::format("while {} {{ {} }}", condition->to_string(), body->to_string());
  }

  Option<meta::VariantType> While::variant_type() const {
    return crab::some(meta::VariantType::UNIT);
  }
} // namespace goos::ast::expression
