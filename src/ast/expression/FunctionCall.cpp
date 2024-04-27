//
// Created by bishan_ on 4/26/24.
//

#include "FunctionCall.hpp"

#include <sstream>
#include <fmt/core.h>

namespace goos::ast::expression {
  FunctionCall::FunctionCall(Box<Expression> function, Vec<Box<Expression>> arguments)
    : function{std::move(function)},
      arguments{std::move(arguments)} {}

  String FunctionCall::to_string() const {
    std::stringstream stream{};

    for (const auto &arg: arguments) {
      stream << fmt::format("({}), ", arg->to_string());
    }

    return fmt::format("funcall({})({})", function->to_string(), stream.str());
  }

  Option<meta::VariantType> FunctionCall::variant_type() const { return crab::none; }
}
