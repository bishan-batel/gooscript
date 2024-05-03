//
// Created by bishan_ on 5/2/24.
//

#include "VariableDeclare.hpp"
#include <fmt/format.h>

namespace goos::ast {
  VariableDeclare::VariableDeclare(String identifier) : identifier{std::move(identifier)} {}

  const String& VariableDeclare::get_identifier() { return identifier; }

  String VariableDeclare::to_string() const {
    return fmt::format("let {}", identifier);
  }
}
