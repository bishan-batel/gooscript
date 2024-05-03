//
// Created by bishan_ on 4/25/24.
//

#include "Expression.hpp"

Box<goos::ast::Statement> goos::ast::Expression::clone() const { return clone_expr(); }
