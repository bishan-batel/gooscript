//
// Created by bishan_ on 4/26/24.
//

#include "Unit.hpp"

namespace goos::ast::expression {
  Unit::Unit() = default;

  String Unit::to_string() const {
    return "unit";
  }

  Option<meta::VariantType> Unit::variant_type() const {
    return crab::some(meta::VariantType::UNIT);
  }
}
