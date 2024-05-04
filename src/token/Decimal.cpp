//
// Created by bishan_ on 4/25/24.
//

#include "Decimal.hpp"

namespace goos::token {
  Decimal::Decimal(const f64 num) : number{num} {}

  f64 Decimal::get_number() const { return number; }

  Box<Token> Decimal::clone() const { return crab::make_box<Decimal>(number); }

  String Decimal::to_string() const {
    return fmt::format("{}D", number);
  }

  bool Decimal::operator==(const Token &other) const {
    if (const Option<Ref<Decimal>> num = crab::ref::cast<Decimal>(other)) {
      return num.get_unchecked()->number == number;
    }
    return false;
  }
}
