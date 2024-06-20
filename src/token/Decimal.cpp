//
// Created by bishan_ on 4/25/24.
//

#include "Decimal.hpp"
#include <fmt/xchar.h>

namespace goos::token {
  Decimal::Decimal(SourceFile file, const Range<> range, const f64 number)
    : Token{std::move(file), range}, number{number} {}

  auto Decimal::get_number() const -> f64 { return number; }

  auto Decimal::to_string() const -> WideString {
    return fmt::format(L"{}D", number);
  }

  auto Decimal::operator==(const Token &other) const -> bool {
    if (const Option<Ref<Decimal>> num = crab::ref::cast<Decimal>(other)) {
      return num.get_unchecked()->number == number;
    }
    return false;
  }
}
