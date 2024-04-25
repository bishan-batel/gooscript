//
// Created by bishan_ on 4/25/24.
//

#include "VariableDefinition.hpp"

#include <stdexcept>
#include <utility>

namespace goos::meta {
  VariableDefinition::VariableDefinition(String name) : name(std::move(name)) {}

  const String &VariableDefinition::get_name() const {
    return name;
  }
} // goos
