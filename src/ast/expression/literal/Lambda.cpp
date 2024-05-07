//
// Created by bishan_ on 4/25/24.
//

#include "Lambda.hpp"

#include <algorithm>
#include <ranges>

#include <sstream>
#include <fmt/format.h>
#include <fmt/xchar.h>

namespace goos::ast::expression {
  Lambda::Lambda(Vec<WideString> params, Box<Expression> body)
    : params{std::move(params)}, body{std::move(body)} {}

  auto Lambda::to_string() const -> WideString {
    WideStringStream stream{};

    for (const auto &param: params) {
      stream << fmt::format(L"{}, ", param);
    }
    return fmt::format(L"fn ({}) = {}", stream.str(), body->to_string());
  }

  auto Lambda::get_body() const -> const Expression& {
    return body;
  }

  auto Lambda::clone_expr() const -> Box<Expression> {
    return crab::make_box<Lambda>(params, body->clone_expr());
  }

  auto Lambda::operator==(const Statement &statement) const -> bool {
    auto other_opt = crab::ref::cast<Lambda>(statement);
    if (other_opt.is_none()) return false;

    const Lambda &lambda = other_opt.take_unchecked();

    if (lambda.params.size() != params.size()) return false;
    if (*lambda.body != *body) return false;

    for (usize i = 0; i < params.size(); i++) {
      if (params[i] != lambda.params[i]) return false;
    }
    return true;
  }

  auto Lambda::get_params() const -> const Vec<WideString>& {
    return params;
  }
}
