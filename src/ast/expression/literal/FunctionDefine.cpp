//
// Created by bishan_ on 4/25/24.
//

#include "FunctionDefine.hpp"

#include <sstream>
#include <fmt/compile.h>

namespace goos::ast::expression {
  FunctionDefine::FunctionDefine(Option<String> name, Vec<meta::VariableDefinition> params, Box<Expression> body)
    : name(std::move(name)), params{std::move(params)}, body{std::move(body)} {}

  String FunctionDefine::to_string() const {
    std::stringstream stream{};

    for (const auto &param: params) {
      stream << fmt::format("{}, ", param.get_name());
    }
    return fmt::format("fn ({}) = {}", stream.str(), body->to_string());
  }

  Option<meta::VariantType> FunctionDefine::variant_type() const {
    return crab::some(meta::VariantType::FUNCTION);
  }

  const Expression& FunctionDefine::get_body() const { return body; }

  const Vec<meta::VariableDefinition>& FunctionDefine::get_params() const { return params; }
}
