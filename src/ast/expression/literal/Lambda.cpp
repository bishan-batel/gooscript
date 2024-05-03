//
// Created by bishan_ on 4/25/24.
//

#include "Lambda.hpp"

#include <sstream>
#include <fmt/compile.h>

namespace goos::ast::expression {
  Lambda::Lambda(Vec<String> params, Box<Expression> body)
    : params{std::move(params)}, body{std::move(body)} {}

  String Lambda::to_string() const {
    std::stringstream stream{};

    for (const auto &param: params) {
      stream << fmt::format("{}, ", param);
    }
    return fmt::format("fn ({}) = {}", stream.str(), body->to_string());
  }

  const Expression& Lambda::get_body() const { return body; }

  Box<Expression> Lambda::clone_expr() const {
    return crab::make_box<Lambda>(params, body->clone_expr());
  }

  const Vec<String>& Lambda::get_params() const { return params; }
}
