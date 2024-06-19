//
// Created by bishan_ on 6/19/24.
//

#pragma once
#include "ast/Expression.hpp"

namespace goos::parser::pass::expr {
  class Match final : public ast::Expression {
  public:
    struct Case {
      Box<Expression> state;
      Box<Expression> then;
    };

  private:
    Box<Expression> state;
    Vec<Case> cases;
    Box<Expression> default_case;

  public:
    Match(Box<Expression> state, Vec<Case> cases, Box<Expression> default_case);

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> runtime::Result<runtime::Any> override;
  };
}
