//
// Created by bishan_ on 4/26/24.
//

#include "Decimal.hpp"

#include <fmt/format.h>
#include <fmt/xchar.h>

namespace goos::ast::expression {
  Decimal::Decimal(const f64 literal): literal{literal} {}

  f64 Decimal::get_number() const { return literal; }

  WideString Decimal::to_string() const {
    return fmt::format(L"{}D", literal);
  }

  Box<Expression> Decimal::clone_expr() const {
    return crab::make_box<Decimal>(literal);
  }

  bool Decimal::operator==(const Statement &statement) const {
    if (auto other = crab::ref::cast<Decimal>(statement)) {
      return other.take_unchecked()->literal == literal;
    }
    return false;
  }
}
