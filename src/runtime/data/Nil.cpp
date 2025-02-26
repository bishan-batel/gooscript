//
// Created by bishan_ on 5/15/24.
//

#include "Nil.hpp"
#include <preamble.hpp>
#include "json/utils.hpp"

namespace goos::runtime {
  auto Nil::value() -> RcMut<Nil> {
    static const RcMut<Nil> VALUE = RcMut<Nil>::from_owned_unchecked(new Nil{});
    return VALUE;
  }

  auto Nil::ok() -> Result<std::any, Box<crab::Error>> { return std::any{Any{value()}}; }

  Nil::Nil(std::nullptr_t) : Nil{} {}

  auto Nil::to_string() const -> WideString { return L"nil"; }

  auto Nil::get_type() const -> meta::VariantType { return meta::VariantType::NIL; }

  auto Nil::base_hash() const -> usize { return 42069_u64; }

  auto Nil::clone() const -> Any { return value(); }

  auto Nil::to_json() const -> Box<json::Value> { return json::to_json("nil"); }
} // namespace goos::runtime
