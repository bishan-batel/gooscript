//
// Created by bishan_ on 4/26/24.
//

#include "String.hpp"
#include <fmt/core.h>

namespace goos::ast::expression {
  String::String(::String literal) : literal(std::move(literal)) {}

  const ::String& String::get_string() const { return literal; }

  ::String String::to_string() const {
    return fmt::format("\"{}\"", literal);
  }

  Option<meta::VariantType> String::variant_type() const {
    return crab::some(meta::VariantType::STRING);
  }
}
