//
// Created by bishan_ on 4/25/24.
//

#include "Lambda.hpp"

#include <sstream>
#include <fmt/format.h>
#include <fmt/xchar.h>

namespace goos::ast::expression {
  Lambda::Lambda(Vec<WideString> params, Box<Expression> body)
    : params{std::move(params)}, body{std::move(body)} {}

  WideString Lambda::to_string() const {
    WideStringStream stream{};

    for (const auto &param: params) {
      stream << fmt::format(L"{}, ", param);
    }
    return fmt::format(L"fn ({}) = {}", stream.str(), body->to_string());
  }

  const Expression& Lambda::get_body() const { return body; }

  Box<Expression> Lambda::clone_expr() const {
    return crab::make_box<Lambda>(params, body->clone_expr());
  }

  const Vec<WideString>& Lambda::get_params() const { return params; }
}
