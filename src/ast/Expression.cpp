//
// Created by bishan_ on 4/25/24.
//

#include "Expression.hpp"

auto goos::ast::Expression::clone() const -> Box<Statement> { return clone_expr(); }

auto goos::ast::Expression::accept(Statement::IVisitor &visitor) const -> void {
  if (const auto expr_visitor = dynamic_cast<IVisitor*>(&visitor)) {
    [[maybe_unused]] auto _ = accept_expr(*expr_visitor);
  }
}
