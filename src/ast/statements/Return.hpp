//
// Created by bishan_ on 5/1/24.
//

#pragma once
#include <box.hpp>
#include "ast/Expression.hpp"

namespace goos::ast {
  class Return final : public Statement {
      Box<Expression> expression;

    public:
      explicit Return(Box<Expression> expr);

      [[nodiscard]] const Expression &get_expression() const;
      [[nodiscard]] WideString to_string() const override;

      [[nodiscard]] Box<Statement> clone() const override;

      [[nodiscard]] bool operator==(const Statement &statement) const override;
  };
} // namespace goos::ast
