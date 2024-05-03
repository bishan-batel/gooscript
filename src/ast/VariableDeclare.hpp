//
// Created by bishan_ on 5/2/24.
//

#pragma once

#include "Statement.hpp"

namespace goos::ast {
  class VariableDeclare final : public Statement {
    String identifier;

  public:
    explicit VariableDeclare(String identifier);

    [[nodiscard]] const String& get_identifier();

    [[nodiscard]] String to_string() const override;
  };
}
