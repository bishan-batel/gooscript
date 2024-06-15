//
// Created by bishan_ on 6/14/24.
//

#include "StringLiteral.hpp"

#include <fmt/xchar.h>

namespace goos::runtime {
  StringLiteral::StringLiteral(WideString string): text{std::move(string)} {}

  auto StringLiteral::set(WideString v) -> void {
    text = std::move(v);
  }

  auto StringLiteral::get() const -> const WideString& {
    return text;
  }

  auto StringLiteral::to_string() const -> WideString {
    return text;
  }

  auto StringLiteral::get_type() const -> meta::VariantType {
    return meta::VariantType::STRING;
  }
}
