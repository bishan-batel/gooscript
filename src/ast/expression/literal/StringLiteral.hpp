//
// Created by bishan_ on 4/26/24.
//

#pragma once
#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class StringLiteral final : public Expression {
    Rc<WideString> literal;

  public:
    explicit StringLiteral(Rc<WideString> literal);

    [[nodiscard]] auto get_string() const -> Rc<WideString>;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> runtime::Result<runtime::Any> override;
  };
}
