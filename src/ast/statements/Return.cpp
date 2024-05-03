//
// Created by bishan_ on 5/1/24.
//

#include "Return.hpp"

#include <fmt/format.h>

namespace goos::ast {
  Return::Return(Box<Expression> expr) : expression{std::move(expr)} {}

  const Expression& Return::get_expression() const { return expression; }

  String Return::to_string() const { return fmt::format("return {}", expression->to_string()); }

  Box<Statement> Return::clone() const {
    return crab::make_box<Return>(expression->clone_expr());
  }
} // namespace goos::ast
