//
// Created by bishan_ on 6/15/24.
//

#include "InvalidCast.hpp"

namespace goos::runtime {
  err::InvalidCast::InvalidCast(const meta::VariantType from, const meta::VariantType to)
    : from{from},
      to{to} {}

  auto err::InvalidCast::get_from() const -> meta::VariantType { return from; }

  auto err::InvalidCast::get_to() const -> meta::VariantType { return to; }

  auto err::InvalidCast::what() const -> String {
    return fmt::format(
      "Cannot convert from [{}] to [{}]",
      (StringStream{} << get_from()).str(),
      (StringStream{} << get_to()).str()
    );
  }
}
