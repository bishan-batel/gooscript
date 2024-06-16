//
// Created by bishan_ on 6/15/24.
//

#pragma once
#include "RuntimeError.hpp"
#include "meta/VariantType.hpp"

namespace goos::runtime::err {
  class InvalidCast final : public Error {
    meta::VariantType from, to;

  public:
    InvalidCast(meta::VariantType from, meta::VariantType to);

    [[nodiscard]] auto get_from() const -> meta::VariantType;

    [[nodiscard]] auto get_to() const -> meta::VariantType;

    [[nodiscard]] auto what() const -> String override;
  };
}
