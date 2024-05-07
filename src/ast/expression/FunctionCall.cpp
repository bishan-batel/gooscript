//
// Created by bishan_ on 4/26/24.
//

#include "FunctionCall.hpp"

#include <sstream>
#include <fmt/format.h>
#include <fmt/xchar.h>

namespace goos::ast::expression {
  FunctionCall::FunctionCall(Box<Expression> function, Vec<Box<Expression>> arguments)
    : function{std::move(function)},
      arguments{std::move(arguments)} {}

  WideString FunctionCall::to_string() const {
    WideStringStream stream{};

    for (const auto &arg: arguments) {
      stream << fmt::format(L"({}), ", arg->to_string());
    }

    return fmt::format(L"funcall({})({})", function->to_string(), stream.str());
  }

  Box<Expression> FunctionCall::clone_expr() const {
    Vec<Box<Expression>> arguments_clone{};

    for (const auto &arg: arguments) {
      arguments_clone.push_back(arg->clone_expr());
    }

    return crab::make_box<FunctionCall>(function->clone_expr(), std::move(arguments_clone));
  }
}
