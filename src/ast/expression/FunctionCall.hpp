//
// Created by bishan_ on 4/26/24.
//

#pragma once
#include <ast/Expression.hpp>
#include <box.hpp>

namespace goos::ast::expression {
  class FunctionCall final : public Expression {
    Box<Expression> function;
    Vec<Box<Expression>> arguments;

  public:
    FunctionCall(Box<Expression> function, Vec<Box<Expression>> arguments);

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> std::shared_ptr<runtime::Value> override;

    [[nodiscard]] auto get_function() const -> const Expression&;

    [[nodiscard]] auto get_arguments() const -> const Vec<Box<Expression>>&;
  };
}
