//
// Created by bishan_ on 5/15/24.
//

#include <preamble.hpp>
#include "Nil.hpp"

namespace goos::runtime {
  auto Nil::value() -> RcMut<Nil> {
    static const RcMut<Nil> VALUE = crab::make_rc_mut<Nil>();
    return VALUE;
  }

  Nil::Nil(std::nullptr_t) : Nil{} {}

  auto Nil::to_string() const -> WideString {
    return L"nil";
  }

  auto Nil::get_type() const -> meta::VariantType {
    return meta::VariantType::NIL;
  }

  auto Nil::base_hash() const -> usize {
    return 42069_u64;
  }

  auto Nil::clone() const -> Any {
    return crab::make_rc_mut<Nil>();
  }
}
