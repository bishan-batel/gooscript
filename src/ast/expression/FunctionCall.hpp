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
    TokenTrace trace;

  public:
    FunctionCall(Box<Expression> function, Vec<Box<Expression>> arguments, TokenTrace trace)
        : function{std::move(function)}, arguments{std::move(arguments)}, trace{trace} {}

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto clone_expr() const -> Box<Expression> override;

    [[nodiscard]] auto operator==(const Statement &statement) const -> bool override;

    [[nodiscard]] auto json() const -> Box<json::Value> override;

    [[nodiscard]] auto accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> override;

    [[nodiscard]] auto get_function() const -> const Expression &;

    [[nodiscard]] auto get_arguments() const -> const Vec<Box<Expression>> &;

    [[nodiscard]] auto token_trace() const -> TokenTrace override { return trace; }
  };
} // namespace goos::ast::expression
