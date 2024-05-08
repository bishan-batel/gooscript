//
// Created by bishan_ on 5/7/24.
//

#pragma once
#include "ast/Expression.hpp"
#include "ast/Statement.hpp"
#include "meta/Mutability.hpp"

namespace goos::ast {
  class VariableDeclaration final : public Statement {
    WideString name;
    meta::Mutability mutability;
    Box<Expression> initializer;

  public:
    explicit VariableDeclaration(WideString name, meta::Mutability mutability, Box<Expression> initializer);

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone() const -> Box<Statement> override;

    [[nodiscard]] auto get_mutability() const -> meta::Mutability;

    [[nodiscard]] auto get_initializer() const -> const Expression&;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;
  };
}
