//
// Created by bishan_ on 4/26/24.
//

#include "ScopeBlock.hpp"

#include <sstream>
#include <algorithm>
#include <ranges>

#include "ast/expression/literal/Unit.hpp"
#include "ast/statements/Eval.hpp"
#include <fmt/format.h>
#include <fmt/xchar.h>

#include "json/Array.hpp"
#include "json/Object.hpp"

namespace goos::ast::expression {
  ScopeBlock::ScopeBlock(Vec<Box<Statement>> statements)
    : statements{std::move(statements)}, eval{crab::make_box<Unit>()} {}

  ScopeBlock::ScopeBlock(Vec<Box<Statement>> statements, Box<Expression> eval)
    : statements{std::move(statements)}, eval{std::move(eval)} {}

  auto ScopeBlock::to_string() const -> WideString {
    WideStringStream stream{};

    for (const auto &statement: statements) {
      stream << statement->to_string() << "; ";
    }

    stream << eval->to_string() << ";";

    return fmt::format(L"block {{ {} }}", stream.str());
  }

  auto ScopeBlock::clone_expr() const -> Box<Expression> {
    Vec<Box<Statement>> statements{};

    for (const auto &statement: statements) {
      statements.push_back(statement->clone());
    }

    return crab::make_box<ScopeBlock>(std::move(statements));
  }

  auto ScopeBlock::operator==(const Statement &statement) const -> bool {
    auto other_result = crab::ref::cast<ScopeBlock>(statement);
    if (other_result.is_none()) return false;

    const Ref<ScopeBlock> other{other_result.take_unchecked()};

    if (statements.size() != other->statements.size()) return false;

    if (*eval != *other->eval) return false;

    for (usize i = 0; i < statements.size(); i++) {
      if (*statements[i] != *other->statements[i]) return false;
    }
    return true;
  }

  auto ScopeBlock::json() const -> Box<json::Value> {
    auto obj = crab::make_box<json::Object>();
    obj->put(L"type", L"scope");

    auto arr = crab::make_box<json::Array>();
    for (const auto &statement: statements) {
      arr->push(statement->json());
    }

    obj->put(L"statements", std::move(arr));
    obj->put(L"eval", eval->json());

    return obj;
  }

  auto ScopeBlock::accept_expr(IVisitor &visitor) const -> runtime::Result<runtime::Any> {
    return visitor.visit_scope(*this);
  }

  auto ScopeBlock::get_statements() const -> const Vec<Box<Statement>>& { return statements; }

  auto ScopeBlock::get_eval() const -> const Expression& { return eval; };
}
