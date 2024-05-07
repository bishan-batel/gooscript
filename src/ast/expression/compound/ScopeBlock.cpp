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

namespace goos::ast::expression {
  ScopeBlock::ScopeBlock(Vec<Box<Statement>> statements)
    : statements{std::move(statements)}, eval{crab::make_box<Eval>(crab::make_box<Unit>())} {}

  ScopeBlock::ScopeBlock(Vec<Box<Statement>> statements, Box<Eval> eval)
    : statements{std::move(statements)}, eval{std::move(eval)} {}

  WideString ScopeBlock::to_string() const {
    WideStringStream stream{};

    for (const auto &statement: statements) {
      stream << statement->to_string() << ";\n";
    }

    return fmt::format(L"block {{ {} }}", stream.str());
  }

  Box<Expression> ScopeBlock::clone_expr() const {
    Vec<Box<Statement>> statements{};

    for (const auto &statement: statements) {
      statements.push_back(statement->clone());
    }

    return crab::make_box<ScopeBlock>(std::move(statements));
  }

  bool ScopeBlock::operator==(const Statement &statement) const {
    auto other_result = crab::ref::cast<ScopeBlock>(statement);
    if (other_result.is_none()) return false;

    const ScopeBlock &other = other_result.take_unchecked();

    if (statements.size() != other.statements.size()) return false;

    if (*eval != *other.eval) return false;

    for (usize i = 0; i < statements.size(); i++) {
      if (*statements[i] != *other.statements[i]) return false;
    }
    return true;
  };
}
