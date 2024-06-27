//
// Created by bishan_ on 4/25/24.
//

#include "Binary.hpp"

#include <fmt/format.h>
#include <fmt/xchar.h>

#include "json/Object.hpp"

namespace goos::ast::expression {
  Binary::Binary(Box<Expression> lhs, const lexer::Operator op, Box<Expression> rhs)
    : lhs(std::move(lhs)), rhs(std::move(rhs)), op(op) {}

  auto Binary::get_lhs() const -> const Expression& {
    return lhs;
  }

  auto Binary::get_rhs() const -> const Expression& {
    return rhs;
  }

  auto Binary::get_op() const -> const lexer::Operator& {
    return op;
  }

  auto Binary::to_string() const -> WideString {
    return fmt::format(
      L"({}) {} ({})",
      lhs->to_string(),
      lexer::OPERATOR_TO_STR_MAP.at(op),
      rhs->to_string()
    );
  }

  auto Binary::clone_expr() const -> Box<Expression> {
    return crab::make_box<Binary>(lhs->clone_expr(), op, rhs->clone_expr());
  }

  auto Binary::operator==(const Statement &statement) const -> bool {
    auto other_opt = crab::ref::cast<Binary>(statement);
    if (other_opt.is_none()) return false;

    const Ref<Binary> other{other_opt.take_unchecked()};
    return other->op == op and other->get_lhs() == lhs and other->get_rhs() == rhs;
  }

  auto Binary::json() const -> Box<json::Value> {
    auto obj = crab::make_box<json::Object>();
    obj->put(L"op", WideString{lexer::OPERATOR_TO_STR_MAP.at(op)});
    obj->put(L"lhs", lhs->json());
    obj->put(L"rhs", rhs->json());
    return obj;
  }

  auto Binary::accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> {
    return visitor.visit_binary(*this);
  }
}
