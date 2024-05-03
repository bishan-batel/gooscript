//
// Created by bishan_ on 4/25/24.
//

#include "Lambda.hpp"

#include <sstream>
#include <fmt/compile.h>

namespace goos::ast::expression {
  Lambda::Lambda(Vec<meta::VariableDefinition> params, Box<Expression> body)
    : params{std::move(params)}, body{std::move(body)} {}

  String Lambda::to_string() const {
    std::stringstream stream{};

    for (const auto &param: params) {
      stream << fmt::format("{}, ", param.get_name());
    }
    return fmt::format("fn ({}) = {}", stream.str(), body->to_string());
  }

  Option<meta::VariantType> Lambda::variant_type() const {
    return crab::some(meta::VariantType::FUNCTION);
  }

  const Expression& Lambda::get_body() const { return body; }

  const Vec<meta::VariableDefinition>& Lambda::get_params() const { return params; }
}
