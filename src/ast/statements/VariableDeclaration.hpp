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

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone() const -> Box<Statement> override;

    [[nodiscard]] auto get_mutability() const -> meta::Mutability;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;
  };
}
