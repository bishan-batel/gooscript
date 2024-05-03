//
// Created by bishan_ on 5/1/24.
//

#include "If.hpp"

#include <fmt/format.h>

namespace goos::ast::expression {
  If::If(Box<Expression> condition, Box<Expression> then, Box<Expression> else_then)
      : condition{std::move(condition)}, then{std::move(then)}, else_then{std::move(else_then)} {}

  String If::to_string() const {
    return fmt::format(
        "if {} then {{ {} }} else {{ {} }}", condition->to_string(), then->to_string(), else_then->to_string());
  }

  Option<meta::VariantType> If::variant_type() const { return then->variant_type(); }
} // namespace goos::ast::expression
