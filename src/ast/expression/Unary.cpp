//
// Created by bishan_ on 4/25/24.
//

#include "Unary.hpp"
#include <fmt/compile.h>

namespace goos::ast::expression {
  Unary::Unary(const lexer::Operator op, Box<Expression> expr)
    : expr{std::move(expr)}, op{op} {}

  const Expression &Unary::get_expression() const { return expr; }

  String Unary::to_string() const {
    return fmt::format("{} ({})", lexer::OPERATOR_TO_STR_MAP.at(op), expr->to_string());
  }

  Option<meta::VariantType> Unary::variant_type() {
    return expr->variant_type();
  }
}
