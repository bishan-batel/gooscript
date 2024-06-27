//
// Created by bishan_ on 6/17/24.
//

#pragma once
#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class ArrayIndex final : public Expression {
    Box<Expression> object, index;

  public:
    ArrayIndex(Box<Expression> object, Box<Expression> index);

    [[nodiscard]] auto get_object() const -> const Expression&;

    [[nodiscard]] auto get_index() const -> const Expression&;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> override;
  };
}
