//
// Created by bishan_ on 5/1/24.
//

#include "Return.hpp"

#include <fmt/format.h>
#include <fmt/xchar.h>

namespace goos::ast {
  Return::Return(Box<Expression> expr) : expression{std::move(expr)} {}

  auto Return::get_expression() const -> const Expression& { return expression; }

  auto Return::to_string() const -> WideString { return fmt::format(L"return {}", expression->to_string()); }

  auto Return::clone() const -> Box<Statement> {
    return crab::make_box<Return>(expression->clone_expr());
  }

  auto Return::operator==(const Statement &statement) const -> bool {
    if (auto other{crab::ref::cast<Return>(statement)}) {
      return *expression == *other.take_unchecked()->expression;
    }
    return false;
  }
} // namespace goos::ast
