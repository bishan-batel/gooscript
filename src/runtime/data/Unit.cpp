//
// Created by bishan_ on 5/16/24.
//

#include "Unit.hpp"
#include "json/utils.hpp"

#include <any>
#include <box.hpp>

namespace goos::runtime {
  auto Unit::value() -> RcMut<Unit> {
    static const RcMut<Unit> VALUE = crab::make_rc_mut<Unit>();
    return VALUE;
  }

  auto Unit::ok() -> Result<std::any, Box<crab::Error>> { return std::any{Any{value()}}; }

  Unit::Unit() = default;

  auto Unit::to_string() const -> WideString { return L"unit"; }

  auto Unit::get_type() const -> meta::VariantType { return meta::VariantType::UNIT; }

  auto Unit::base_hash() const -> usize { return 4206942_u64; }

  auto Unit::clone() const -> Any { return value(); }

  auto Unit::to_json() const -> Box<json::Value> { return json::to_json(L"unit"); }
} // namespace goos::runtime
