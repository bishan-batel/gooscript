//
// Created by bishan_ on 4/26/24.
//

#include "FunctionCall.hpp"

#include <sstream>
#include <fmt/format.h>
#include <fmt/xchar.h>

#include "json/Array.hpp"
#include "json/Object.hpp"
#include "literal/Array.hpp"

namespace goos::ast::expression {
  FunctionCall::FunctionCall(Box<Expression> function, Vec<Box<Expression>> arguments)
    : function{std::move(function)},
      arguments{std::move(arguments)} {}

  auto FunctionCall::to_string() const -> WideString {
    WideStringStream stream{};

    for (const auto &arg: arguments) {
      stream << fmt::format(L"({}), ", arg->to_string());
    }

    return fmt::format(L"funcall({})({})", function->to_string(), stream.str());
  }

  auto FunctionCall::clone_expr() const -> Box<Expression> {
    Vec<Box<Expression>> arguments_clone{};

    for (const auto &arg: arguments) {
      arguments_clone.push_back(arg->clone_expr());
    }

    return crab::make_box<FunctionCall>(function->clone_expr(), std::move(arguments_clone));
  }

  auto FunctionCall::operator==(const Statement &statement) const -> bool {
    Option<Ref<FunctionCall>> other_opt = crab::ref::cast<FunctionCall>(statement);
    if (other_opt.is_none()) return false;

    const Ref<FunctionCall> function{other_opt.take_unchecked()};

    if (function->arguments.size() != arguments.size()) return false;
    if (*function->function != *this->function) return false;

    for (usize i = 0; i < arguments.size(); i++) {
      if (*arguments[i] != *function->arguments[i]) return false;
    }
    return true;
  }

  auto FunctionCall::json() const -> Box<json::Value> {
    auto obj = crab::make_box<json::Object>();
    obj->put(L"type", L"funcall");

    obj->put(L"function", function->json());

    auto args = crab::make_box<json::Array>();
    for (const auto &p: arguments) {
      args->push(p->json());
    }

    obj->put(L"args", std::move(args));

    return obj;
  }

  auto FunctionCall::accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> {
    return visitor.visit_function_call(*this);
  }

  auto FunctionCall::get_function() const -> const Expression& { return function; }

  auto FunctionCall::get_arguments() const -> const Vec<Box<Expression>>& { return arguments; }
}
