//
// Created by bishan_ on 4/26/24.
//

#include "ScopeBlock.hpp"

#include <sstream>

#include "../../../../build/Release/_deps/fmt-src/include/fmt/format.h"
#include "ast/expression/literal/Unit.hpp"
#include "ast/statements/Eval.hpp"

goos::ast::expression::ScopeBlock::ScopeBlock(Vec<Box<Statement>> statements)
  : statements{std::move(statements)}, eval{crab::make_box<Eval>(crab::make_box<Unit>())} {}

goos::ast::expression::ScopeBlock::ScopeBlock(Vec<Box<Statement>> statements, Box<Eval> eval)
  : statements{std::move(statements)}, eval{std::move(eval)} {}

String goos::ast::expression::ScopeBlock::to_string() const {
  std::stringstream stream{};

  for (const auto &statement: statements) {
    stream << statement->to_string() << ";\n";
  }

  return fmt::format("scope {{ {} }}", stream.str());
}

Option<goos::meta::VariantType> goos::ast::expression::ScopeBlock::variant_type() const {
  return eval->get_expr().variant_type();
}
