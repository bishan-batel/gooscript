//
// Created by bishan_ on 4/26/24.
//

#include "Decimal.hpp"

#include <fmt/compile.h>

namespace goos::ast::expression {
  Decimal::Decimal(const f64 literal): literal{literal} {}

  f64 Decimal::get_number() const { return literal; }

  String Decimal::to_string() const {
    return fmt::format("{}D", literal);
  }

  Box<Expression> Decimal::clone_expr() const {
    return crab::make_box<Decimal>(literal);
  }
}
