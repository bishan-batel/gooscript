//
// Created by bishan_ on 4/27/24.
//

#include "Nil.hpp"

namespace goos::ast::expression {
  Nil::Nil() {
    throw std::logic_error("Not implemented");
  }

  String Nil::to_string() const {
    return "nil";
  }

  Option<meta::VariantType> Nil::variant_type() const {
    return crab::some(meta::VariantType::NIL);
  }
} // goos
