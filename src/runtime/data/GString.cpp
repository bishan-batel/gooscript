//
// Created by bishan_ on 6/14/24.
//

#include "GString.hpp"

#include <fmt/xchar.h>

namespace goos::runtime {
  GString::GString(WideString string): GString{crab::make_rc<WideString>(std::move(string))} {}

  GString::GString(const meta::Identifier &identifier)
    : text{identifier.get_string_ref_counted()}, cached_hash{identifier.get_hash()}, is_hash_dirty{false} {}

  GString::GString(Rc<WideString> string)
    : text{std::move(string)},
      cached_hash{0},
      is_hash_dirty{true} {}

  auto GString::get() const -> const WideString& {
    return text;
  }

  auto GString::to_string() const -> WideString {
    return text;
  }

  auto GString::get_type() const -> meta::VariantType {
    return meta::VariantType::STRING;
  }

  auto GString::base_hash() const -> usize {
    if (is_hash_dirty) {
      cached_hash = std::hash<WideString>{}(text);
      is_hash_dirty = false;
    }
    return cached_hash;
  }

  auto GString::clone() const -> Any {
    return crab::make_rc_mut<GString>(text);
  }
}
