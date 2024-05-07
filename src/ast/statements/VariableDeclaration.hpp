//
// Created by bishan_ on 5/7/24.
//

#pragma once
#include "ast/Statement.hpp"
#include "meta/Mutability.hpp"

namespace goos::ast {
  class VariableDeclaration final : public Statement {
    WideString name;
    meta::Mutability mutability;

  public:
    explicit VariableDeclaration(WideString name, meta::Mutability mutability);

    [[nodiscard]] WideString to_string() const override;

    [[nodiscard]] Box<Statement> clone() const override;

    [[nodiscard]] meta::Mutability get_mutability() const;
  };
}
