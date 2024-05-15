//
// Created by bishan_ on 5/3/24.
//

#pragma once

#include "ast/Expression.hpp"

namespace goos::ast::expression {
  class Dictionary final : public Expression {
  public:
    struct Pair {
      Box<Expression> key;
      Box<Expression> value;
    };

  private:
    Vec<Pair> pairs;

  public:
    explicit Dictionary(Vec<Pair> pairs);

    [[nodiscard]] auto get_pairs() const -> const Vec<Pair>&;

    [[nodiscard]] auto get(const Expression &key) const -> Option<Ref<Pair>>;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> std::shared_ptr<runtime::Value> override;
  };
}
