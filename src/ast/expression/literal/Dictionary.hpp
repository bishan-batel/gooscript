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

    [[nodiscard]] const Vec<Pair>& get_pairs() const;

    [[nodiscard]] Option<Ref<Pair>> get(const Expression& key) const;

    [[nodiscard]] WideString to_string() const override;

    [[nodiscard]] Box<Expression> clone_expr() const override;

    [[nodiscard]] bool operator==(const Statement &statement) const override;
  };
}
