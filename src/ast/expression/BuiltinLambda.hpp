//
// Created by bishan_ on 5/2/24.
//

#pragma once

#include "ast/Expression.hpp"
#include <box.hpp>
#include <functional>

namespace goos::ast::expression {
  class BuiltinLambda final : public Expression {
  public:
    using ArgumentList = const Vec<Box<Expression>>&;
    using Callback = std::function<Box<Expression>(ArgumentList)>;

  private:
    Callback builtin_callback;

  public:
    explicit BuiltinLambda(Callback callback);

    [[nodiscard]] String to_string() const override;

    [[nodiscard]] Option<meta::VariantType> variant_type() const override;
  };
}
