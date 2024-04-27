//
// Created by bishan_ on 4/26/24.
//

#pragma once
#include <box.hpp>

#include "ast/Expression.hpp"
#include "ast/statements/Eval.hpp"

namespace goos::ast::expression {
  class ScopeBlock final : public Expression {
    Vec<Box<Statement>> statements;
    Box<Eval> eval;

  public:
    explicit ScopeBlock(Vec<Box<Statement>> statements);

    explicit ScopeBlock(Vec<Box<Statement>> statements, Box<Eval> eval);

    [[nodiscard]] String to_string() const override;

    [[nodiscard]] Option<meta::VariantType> variant_type() const override;
  };
}
