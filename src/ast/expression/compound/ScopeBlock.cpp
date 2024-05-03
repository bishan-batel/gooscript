//
// Created by bishan_ on 4/26/24.
//

#include "ScopeBlock.hpp"

#include <sstream>

#include "../../../../build/Release/_deps/fmt-src/include/fmt/format.h"
#include "ast/expression/literal/Unit.hpp"
#include "ast/statements/Eval.hpp"

namespace goos::ast::expression {
  ScopeBlock::ScopeBlock(Vec<Box<Statement>> statements)
    : statements{std::move(statements)}, eval{crab::make_box<Eval>(crab::make_box<Unit>())} {}

  ScopeBlock::ScopeBlock(Vec<Box<Statement>> statements, Box<Eval> eval)
    : statements{std::move(statements)}, eval{std::move(eval)} {}

  String ScopeBlock::to_string() const {
    std::stringstream stream{};

    for (const auto &statement: statements) {
      stream << statement->to_string() << ";\n";
    }

    return fmt::format("scope {{ {} }}", stream.str());
  }

  Box<Expression> ScopeBlock::clone_expr() const {
    Vec<Box<Statement>> statements{};

    for (const auto &statement: statements) {
      statements.push_back(statement->clone());
    }

    return crab::make_box<ScopeBlock>(std::move(statements));
  }
}
