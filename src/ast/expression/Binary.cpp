//
// Created by bishan_ on 4/25/24.
//

#include "Binary.hpp"

#include <fmt/compile.h>

namespace goos::ast::expression {
  Binary::Binary(Box<Expression> lhs, const lexer::Operator op, Box<Expression> rhs)
    : lhs(std::move(lhs)), rhs(std::move(rhs)), op(op) {}

  const Expression& Binary::get_lhs() const { return lhs; }

  const Expression& Binary::get_rhs() const { return rhs; }

  String Binary::to_string() const {
    return fmt::format(
      "({}) {} ({})",
      lhs->to_string(),
      lexer::OPERATOR_TO_STR_MAP.at(op),
      rhs->to_string()
    );
  }

  Box<Expression> Binary::clone_expr() const {
    return crab::make_box<Binary>(lhs->clone_expr(), op, rhs->clone_expr());
  }
}
