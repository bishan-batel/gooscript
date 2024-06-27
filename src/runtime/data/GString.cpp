//
// Created by bishan_ on 6/14/24.
//

#include "GString.hpp"

#include <fmt/xchar.h>

#include "TypeConversion.hpp"

namespace goos::runtime {
  GString::GString(WideString string): GString{crab::make_rc<WideString>(std::move(string))} {}

  GString::GString(const WideStringView string) : GString{WideString{string}} {}

  GString::GString(const widechar *string) : GString{WideStringView{string}} {}

  GString::GString(const meta::Identifier &identifier)
    : text{identifier.get_string_ref_counted()},
      cached_hash{identifier.get_hash()} {}

  GString::GString(Rc<WideString> string)
    : text{std::move(string)},
      cached_hash{utils::hash(*this->text)} {}

  auto GString::get() const -> Rc<WideString> {
    return text;
  }

  auto GString::to_string() const -> WideString {
    return *text;
  }

  auto GString::get_type() const -> meta::VariantType {
    return meta::VariantType::STRING;
  }

  auto GString::base_hash() const -> usize {
    return cached_hash;
  }

  auto GString::clone() const -> Any {
    return crab::make_rc_mut<GString>(text);
  }

  auto GString::index(const Any index) -> Result<Any, Box<err::Error>> {
    Result<RcMut<Integer>, Box<err::Error>> integer = type::coerce<Integer>(index);

    if (integer.is_err()) return integer.take_err_unchecked();

    // TODO out of bounds index
    const auto i = static_cast<usize>(integer.take_unchecked()->get());
    return type::to_goos_any(WideStringView{&text->at(i), 1});
  }
}
