//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include <box.hpp>

#include "ast/Expression.hpp"
#include "meta/Identifier.hpp"

namespace goos::ast::expression {
  class Lambda final : public Expression {
    Vec<meta::Identifier> params;
    Box<Expression> body;

  public:
    explicit Lambda(
      Vec<meta::Identifier> params,
      Box<Expression> body
    );

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_params() const -> const Vec<meta::Identifier>&;

    [[nodiscard]] auto get_body() const -> const Expression&;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> runtime::Result<runtime::Any> override;
  };
}
