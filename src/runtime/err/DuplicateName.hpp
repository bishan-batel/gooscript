//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include "RuntimeError.hpp"
#include "meta/Identifier.hpp"

namespace goos::runtime::err {
  class DuplicateName final : public Error {
    meta::Identifier identifier;

  public:
    explicit DuplicateName(meta::Identifier identifier);

    [[nodiscard]] auto get_identifier() const -> meta::Identifier;

    [[nodiscard]] auto what() const -> String override;
  };
}
