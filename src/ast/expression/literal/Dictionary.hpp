//
// Created by bishan_ on 5/3/24.
//

#pragma once

#include "ast/Expression.hpp"
#include "ast/Statement.hpp"

namespace goos::ast::expression {
  class Dictionary final : public Expression {
  public:
    struct Pair {
      Box<Expression> key;
      Box<Expression> value;
    };

  private:
    Vec<Pair> pairs;
    TokenTrace trace;

  public:
    explicit Dictionary(Vec<Pair> pairs, TokenTrace trace) : pairs{std::move(pairs)}, trace{std::move(trace)} {}

    [[nodiscard]] auto get_pairs() const -> const Vec<Pair> &;

    [[nodiscard]] auto get(const Expression &key) const -> Option<Ref<Pair>>;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto token_trace() const -> TokenTrace override { return trace; }

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> override;
  };
} // namespace goos::ast::expression
