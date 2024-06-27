//
// Created by bishan_ on 4/26/24.
//

#include "StringLiteral.hpp"
#include <fmt/format.h>
#include <fmt/xchar.h>

#include "json/Text.hpp"

namespace goos::ast::expression {
  StringLiteral::StringLiteral(Rc<WideString> literal) : literal(std::move(literal)) {}

  auto StringLiteral::get_string() const -> Rc<WideString> { return literal; }

  auto StringLiteral::to_string() const -> WideString {
    return fmt::format(L"\"{}\"", *literal);
  }

  auto StringLiteral::clone_expr() const -> Box<Expression> {
    return crab::make_box<StringLiteral>(literal);
  }

  auto StringLiteral::operator==(const Statement &statement) const -> bool {
    if (auto other = crab::ref::cast<StringLiteral>(statement)) {
      return other.take_unchecked()->literal == literal;
    }
    return false;
  }

  auto StringLiteral::json() const -> Box<json::Value> {
    return crab::make_box<json::Text>(literal);
  }

  auto StringLiteral::accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> {
    return visitor.visit_string_literal(*this);
  }
}
