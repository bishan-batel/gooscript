//
// Created by bishan_ on 5/1/24.
//

#include "Return.hpp"

#include <fmt/format.h>
#include <fmt/xchar.h>

namespace goos::ast {
  Return::Return(Box<Expression> expr) : expression{std::move(expr)} {}

  const Expression& Return::get_expression() const { return expression; }

  WideString Return::to_string() const { return fmt::format(L"return {}", expression->to_string()); }

  Box<Statement> Return::clone() const {
    return crab::make_box<Return>(expression->clone_expr());
  }

  bool Return::operator==(const Statement &statement) const {
    if (auto other{crab::ref::cast<Return>(statement)}) {
      return *expression == *other.take_unchecked()->expression;
    }
    return false;
  }
} // namespace goos::ast
