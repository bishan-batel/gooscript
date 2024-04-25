//
// Created by bishan_ on 4/25/24.
//

#include "Function.hpp"

#include <sstream>
#include <fmt/compile.h>

namespace goos::ast::expression {
  Function::Function(Option<String> name, Vec<meta::VariableDefinition> params, Box<Expression> body)
    : name(std::move(name)), params{std::move(params)}, body{std::move(body)} {}

  const Expression &Function::get_body() const { return body; }

  const Vec<meta::VariableDefinition> &Function::get_params() const { return params; }

  String Function::to_string() const {
    std::stringstream stream{};

    for (const auto &param: params) {
      stream << fmt::format("{}, ", param.get_name());
    }
    return fmt::format("fn ({}) = {}", stream.str(), body->to_string());
  }

  Option<meta::VariantType> Function::variant_type() {
    return body->variant_type();
  }
}
