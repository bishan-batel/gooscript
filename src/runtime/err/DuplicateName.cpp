//
// Created by bishan_ on 6/14/24.
//

#include "DuplicateName.hpp"
#include "utils/str.hpp"

namespace goos::runtime::err {
  DuplicateName::DuplicateName(meta::Identifier identifier): identifier{std::move(identifier)} {}

  auto DuplicateName::get_identifier() const -> meta::Identifier { return identifier; }

  auto DuplicateName::what() const -> String {
    return fmt::format("Duplicate name in the same enviornment: {}", str::convert(identifier.get_string()));
  }
}
