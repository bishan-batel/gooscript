//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include <option.hpp>
#include <preamble.hpp>

#include "Statement.hpp"
#include "meta/VariantType.hpp"

namespace goos::ast {
  class Expression : public Statement {
  public:
    Expression() = default;

    [[nodiscard]] virtual Box<Expression> clone_expr() const = 0;

    [[nodiscard]] Box<Statement> clone() const final;
  };
}
