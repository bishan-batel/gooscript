//
// Created by bishan_ on 5/7/24.
//

#include "VariableDeclaration.hpp"

namespace goos::ast {
  VariableDeclaration::VariableDeclaration(WideString name, const meta::Mutability mutability)
    : name{std::move(name)}, mutability{mutability} {}

  WideString VariableDeclaration::to_string() const {
    return name;
  }

  Box<Statement> VariableDeclaration::clone() const {
    return crab::make_box<VariableDeclaration>(name, mutability);
  }

  meta::Mutability VariableDeclaration::get_mutability() const {
    return mutability;
  }
}
