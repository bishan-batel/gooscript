//
// Created by bishan_ on 5/16/24.
//

#include "Decimal.hpp"

#include <fmt/xchar.h>

namespace goos::runtime {
  Decimal::Decimal(const f64 value): value{value} {}

  auto Decimal::to_string() const -> WideString {
    return fmt::format(L"{}", value);
  }

  auto Decimal::get_type() const -> meta::VariantType {
    return meta::VariantType::DECIMAL;
  }
}
