//
// Created by bishan_ on 4/25/24.
//

#include "Unary.hpp"
#include <fmt/format.h>
#include <fmt/xchar.h>

#include "json/Object.hpp"

namespace goos::ast::expression {
  auto Unary::get_expression() const -> const Expression & { return expr; }

  auto Unary::get_op() const -> lexer::Operator { return op; }

  auto Unary::to_string() const -> WideString {
    return fmt::format(L"{} ({})", lexer::OPERATOR_TO_STR_MAP.at(op), expr->to_string());
  }

  auto Unary::clone_expr() const -> Box<Expression> { return crab::make_box<Unary>(op, expr->clone_expr(), op_trace); }

  auto Unary::operator==(const Statement &statement) const -> bool {
    auto other_opt = crab::ref::cast<Unary>(statement);
    if (other_opt.is_none())
      return false;

    const Ref<Unary> other{other_opt.take_unchecked()};
    return other->op == op and *other->expr == *expr;
  }

  auto Unary::json() const -> Box<json::Value> {
    auto obj = crab::make_box<json::Object>();
    obj->put(L"op", WideString{lexer::OPERATOR_TO_STR_MAP.at(op)});
    obj->put(L"expr", expr->json());
    return obj;
  }

  auto Unary::accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> {
    return visitor.visit_unary(*this);
  }

  auto Unary::token_trace() const -> TokenTrace { return TokenTrace::merge_fold(expr->token_trace(), op_trace); }
} // namespace goos::ast::expression
