//
// Created by bishan_ on 5/15/24.
//

#include "Nil.hpp"

namespace goos::runtime {
  auto Nil::to_string() const -> WideString {
    return L"nil";
  }

  auto Nil::get_type() const -> meta::VariantType {
    return meta::VariantType::NIL;
  }
}
