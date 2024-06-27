//
// Created by bishan_ on 4/26/24.
//


#pragma once
#include "ast/Expression.hpp"

namespace goos::ast::expression {

class Unit final : public Expression {
public:
  Unit();

  [[nodiscard]] auto to_string() const -> WideString override;

  [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

  [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

  [[nodiscard]] auto json() const -> Box<json::Value> override;

  [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> override;
};

}
