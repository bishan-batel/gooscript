//
// Created by bishan_ on 4/25/24.
//

#include "Expression.hpp"

auto goos::ast::Expression::clone() const -> Box<Statement> { return clone_expr(); }

auto goos::ast::Expression::accept // NOLINT(*-convert-member-functions-to-static)
(IVisitor &visitor) const -> Result<unit, Box<crab::Error>> {
  if (auto result = accept_expr(visitor); result.is_err()) {
    return result.take_err_unchecked();
  }
  return unit::val;
}
