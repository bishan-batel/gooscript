//
// Created by bishan_ on 5/1/24.
//

#include "If.hpp"

#include <fmt/format.h>
#include <fmt/xchar.h>

namespace goos::ast::expression {
  If::If(Box<Expression> condition, Box<Expression> then, Box<Expression> else_then)
    : condition{std::move(condition)}, then{std::move(then)}, else_then{std::move(else_then)} {}

  WideString If::to_string() const {
    return fmt::format(
      L"if {} then {{ {} }} else {{ {} }}",
      condition->to_string(),
      then->to_string(),
      else_then->to_string()
    );
  }

  Box<Expression> If::clone_expr() const {
    return crab::make_box<If>(
      condition->clone_expr(),
      then->clone_expr(),
      else_then->clone_expr()
    );
  }
} // namespace goos::ast::expression
