//
// Created by bishan_ on 6/28/24.
//

#include "Value.hpp"

namespace goos::codegen {
  auto Value::type_hash() const -> usize {
    return std::visit(
      crab::cases{[]<typename T>(const T &) { return typeid(T).hash_code(); }},
      variant
    );
  }

  // auto Value::get_id() const -> usize { return id; }

  auto Value::get_variant() const -> const Variant& {
    return variant;
  }
} // goos
