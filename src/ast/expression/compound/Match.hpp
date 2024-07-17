//
// Created by bishan_ on 6/19/24.
//

#pragma once
#include "ast/Expression.hpp"
#include "ast/Statement.hpp"

namespace goos::ast::expression {
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
    TokenTrace trace;

  public:
    Match(Box<Expression> state, Vec<Case> cases, Box<Expression> default_case, TokenTrace trace)
        : state{std::move(state)}, cases{std::move(cases)}, default_case{std::move(default_case)}, trace{trace} {}

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> override;

    [[nodiscard]] auto token_trace() const -> TokenTrace override { return trace; }
  };
} // namespace goos::ast::expression
