//
// Created by bishan_ on 4/25/24.
//

#include "Expression.hpp"

auto goos::ast::Expression::clone() const -> Box<Statement> { return clone_expr(); }
