//
// Created by bishan_ on 5/2/24.
//

#include "IdentifierBinding.hpp"
#include <fmt/format.h>

namespace goos::ast::expression {
  IdentifierBinding::IdentifierBinding(String identifier) : identifier{std::move(identifier)} {}

  const String& IdentifierBinding::get_identifier() const { return identifier; }

  String IdentifierBinding::to_string() const { return identifier; }

  Option<meta::VariantType> IdentifierBinding::variant_type() const { return crab::none; }
}
