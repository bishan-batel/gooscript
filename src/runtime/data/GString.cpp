//
// Created by bishan_ on 6/14/24.
//

#include "GString.hpp"

#include <fmt/xchar.h>

namespace goos::runtime {
  GString::GString(WideString string): text{std::move(string)} {}

  auto GString::set(WideString v) -> void {
    text = std::move(v);
  }

  auto GString::get() const -> const WideString& {
    return text;
  }

  auto GString::to_string() const -> WideString {
    return text;
  }

  auto GString::get_type() const -> meta::VariantType {
    return meta::VariantType::STRING;
  }
}
