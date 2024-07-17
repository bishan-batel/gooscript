//
// Created by bishan_ on 5/7/24.
//

#pragma once
#include "ast/Expression.hpp"
#include "ast/Statement.hpp"
#include "meta/Identifier.hpp"
#include "meta/Mutability.hpp"

namespace goos::ast {
  class VariableDeclaration final : public Statement {
    meta::Identifier name;
    meta::Mutability mutability;
    Box<Expression> initializer;
    TokenTrace trace;

  public:
    explicit VariableDeclaration(
        meta::Identifier name, meta::Mutability mutability, Box<Expression> initializer, TokenTrace trace)
        : name{std::move(name)}, mutability{mutability}, initializer{std::move(initializer)}, trace{trace} {}

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone() const -> Box<Statement> override;

    [[nodiscard]] auto get_name() const -> meta::Identifier;

    [[nodiscard]] auto get_mutability() const -> meta::Mutability;

    [[nodiscard]] auto get_initializer() const -> const Expression &;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    auto accept(IVisitor &visitor) const -> Result<unit, Box<crab::Error>> override;

    [[nodiscard]] auto token_trace() const -> TokenTrace override { return trace; }
  };
} // namespace goos::ast
