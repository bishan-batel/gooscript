//
// Created by bishan_ on 6/14/24.
//

#include "VariableNotFound.hpp"
#include "utils/str.hpp"

namespace goos::runtime::err {
  VariableNotFound::VariableNotFound(meta::Identifier identifier): identifier{std::move(identifier)} {}

  auto VariableNotFound::get_identifier() const -> meta::Identifier { return identifier; }

  auto VariableNotFound::what() const -> String {
    return fmt::format("Failed to find variable: {}", str::convert(identifier.get_string()));
  }
}
