//
// Created by bishan_ on 4/26/24.
//

#include <fmt/format.h>
#include "Eval.hpp"

namespace goos::ast::expression {
  Eval::Eval(Box<Expression> expr): expr{std::move(expr)} {}

  const Expression& Eval::get_expr() const { return expr; }

  String Eval::to_string() const {
    return fmt::format("eval ({})", expr->to_string());
  }
}
