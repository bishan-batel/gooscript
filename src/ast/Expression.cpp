//
// Created by bishan_ on 4/25/24.
//

#include "Expression.hpp"

auto goos::ast::Expression::clone() const -> Box<Statement> { return clone_expr(); }

auto goos::ast::Expression::accept(IVisitor &visitor) const -> runtime::VoidResult {
  if (auto result = accept_expr(visitor); result.is_err()) {
    return some(result.take_err_unchecked());
  }
  return crab::none;
}
