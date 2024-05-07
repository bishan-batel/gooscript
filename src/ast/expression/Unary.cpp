//
// Created by bishan_ on 4/25/24.
//

#include "Unary.hpp"
#include <fmt/format.h>
#include <fmt/xchar.h>

namespace goos::ast::expression {
  Unary::Unary(const lexer::Operator op, Box<Expression> expr)
    : expr{std::move(expr)}, op{op} {}

  const Expression& Unary::get_expression() const { return expr; }

  WideString Unary::to_string() const {
    return fmt::format(L"{} ({})", lexer::OPERATOR_TO_STR_MAP.at(op), expr->to_string());
  }

  Box<Expression> Unary::clone_expr() const {
    return crab::make_box<Unary>(op, expr->clone_expr());
  }

  bool Unary::operator==(const Statement &statement) const {
    auto other_opt = crab::ref::cast<Unary>(statement);
    if (other_opt.is_none()) return false;

    const Unary &other = other_opt.take_unchecked();
    return other.op == op and *other.expr == *expr;
  }
}
