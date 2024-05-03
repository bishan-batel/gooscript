//
// Created by bishan_ on 5/2/24.
//

#pragma once
#include <preamble.hpp>

#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class IdentifierBinding final : public Expression {
    ::String identifier;

  public:
    explicit IdentifierBinding(::String identifier);

    [[nodiscard]] const ::String& get_identifier() const;

    [[nodiscard]] ::String to_string() const override;

    [[nodiscard]] Box<Expression> clone_expr() const override;
  };
}
