//
// Created by bishan_ on 4/26/24.
//

#include "Decimal.hpp"

#include <fmt/format.h>
#include <fmt/xchar.h>

namespace goos::ast::expression {
  Decimal::Decimal(const f64 literal): literal{literal} {}

  auto Decimal::get_number() const -> f64 { return literal; }

  auto Decimal::to_string() const -> WideString {
    return fmt::format(L"{}D", literal);
  }

  auto Decimal::clone_expr() const -> Box<Expression> {
    return crab::make_box<Decimal>(literal);
  }

  auto Decimal::operator==(const Statement &statement) const -> bool {
    if (auto other = crab::ref::cast<Decimal>(statement)) {
      return other.take_unchecked()->literal == literal;
    }
    return false;
  }
}
