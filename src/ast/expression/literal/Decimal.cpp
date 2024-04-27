//
// Created by bishan_ on 4/26/24.
//

#include "Decimal.hpp"

#include <fmt/compile.h>

namespace goos::ast::expression {
  Decimal::Decimal(const f64 literal): literal{literal} {}

  f64 Decimal::get_number() const { return literal; }

  String Decimal::to_string() const {
    return fmt::format("");
  }

  Option<meta::VariantType> Decimal::variant_type() const {
    return crab::some(meta::VariantType::DECIMAL);
  }
}
