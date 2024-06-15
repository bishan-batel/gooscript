//
// Created by bishan_ on 5/16/24.
//

#include "Unit.hpp"

namespace goos::runtime {
  Unit::Unit() = default;

  auto Unit::to_string() const -> WideString {
    return L"unit";
  }

  auto Unit::get_type() const -> meta::VariantType {
    return meta::VariantType::UNIT;
  }

  auto Unit::hash() const -> usize {
    return 4206942_u64;
  }
}
