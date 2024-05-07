//
// Created by bishan_ on 5/2/24.
//

#pragma once
#include <preamble.hpp>

#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class IdentifierBinding final : public Expression {
    WideString identifier;

  public:
    explicit IdentifierBinding(WideString identifier);

    [[nodiscard]] const WideString& get_identifier() const;

    [[nodiscard]] WideString to_string() const override;

    [[nodiscard]] Box<Expression> clone_expr() const override;
  };
}
