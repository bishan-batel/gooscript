//
// Created by bishan_ on 4/26/24.
//

#pragma once
#include <box.hpp>

#include "ast/Expression.hpp"
#include "ast/Statement.hpp"

namespace goos::ast {
  class Eval final : public Statement {
    Box<Expression> expression;

  public:
    explicit Eval(Box<Expression> expr);

    [[nodiscard]] auto get_expression() const -> const Expression&;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone() const -> Box<Statement> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;
  };
}
