//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include "preamble.hpp"

namespace goos::meta {
  class VariableDefinition final {
    String name;

  public:
    explicit VariableDefinition(String name);

    [[nodiscard]] const String& get_name() const;
  };
}

