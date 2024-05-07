//
// Created by bishan_ on 5/7/24.
//

#include "VariableDeclaration.hpp"

#include <fmt/format.h>
#include <fmt/xchar.h>

namespace goos::ast {
  VariableDeclaration::VariableDeclaration(WideString name, const meta::Mutability mutability)
    : name{std::move(name)}, mutability{mutability} {}

  auto VariableDeclaration::to_string() const -> WideString {
    WideStringView ty{};
    switch (mutability) {
      case meta::Mutability::CONSTANT:
        ty = L"const";
        break;
      case meta::Mutability::IMMUTABLE:
        ty = L"let";
        break;
      case meta::Mutability::MUTABLE:
        ty = L"let mut";
        break;
    };
    return fmt::format(L"{} {}", ty, name);
  }

  auto VariableDeclaration::clone() const -> Box<Statement> {
    return crab::make_box<VariableDeclaration>(name, mutability);
  }

  auto VariableDeclaration::get_mutability() const -> meta::Mutability {
    return mutability;
  }

  auto VariableDeclaration::operator==(const Statement &statement) const -> bool {
    if (Option<Ref<VariableDeclaration>> opt = crab::ref::cast<VariableDeclaration>(statement)) {
      const Ref<VariableDeclaration> other{opt.take_unchecked()};
      return other->mutability == mutability and other->name == name;
    }

    return false;
  }
}
