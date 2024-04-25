//
// Created by bishan_ on 4/25/24.
//

#include "Decimal.hpp"

namespace goos::token {
  Decimal::Decimal(const f64 num) : number{num} {}

  f64 Decimal::get_number() const { return number; }

  Box<Token> Decimal::clone() const { return crab::make_box<Decimal>(*this); }

  String Decimal::to_string() const {
    return fmt::format("{}D", number);
  }
}
