//
// Created by bishan_ on 4/26/24.
//

#include <fmt/format.h>
#include "Eval.hpp"

namespace goos::ast {
  Eval::Eval(Box<Expression> expr): expression{std::move(expr)} {}

  const Expression& Eval::get_expression() const { return expression; }

  String Eval::to_string() const {
    return fmt::format("eval ({})", expression->to_string());
  }

  Box<Statement> Eval::clone() const { return crab::make_box<Eval>(expression->clone_expr()); }
}
