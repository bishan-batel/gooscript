//
// Created by bishan_ on 5/7/24.
//

#include "VariableDeclaration.hpp"

#include <fmt/format.h>
#include <fmt/xchar.h>

#include "json/Object.hpp"

namespace goos::ast {
  VariableDeclaration::VariableDeclaration(
    WideString name,
    const meta::Mutability mutability,
    Box<Expression> initializer
  )
    : name{std::move(name)}, mutability{mutability}, initializer{std::move(initializer)} {}

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
    return crab::make_box<VariableDeclaration>(name, mutability, initializer->clone_expr());
  }

  auto VariableDeclaration::get_mutability() const -> meta::Mutability {
    return mutability;
  }

  auto VariableDeclaration::get_initializer() const -> const Expression& { return initializer; }

  auto VariableDeclaration::operator==(const Statement &statement) const -> bool {
    if (Option<Ref<VariableDeclaration>> opt = crab::ref::cast<VariableDeclaration>(statement)) {
      const Ref<VariableDeclaration> other{opt.take_unchecked()};
      return other->mutability == mutability and other->name == name;
    }

    return false;
  }

  auto VariableDeclaration::json() const -> Box<json::Value> {
    auto obj = crab::make_box<json::Object>();
    obj->put(L"type", L"declvar");
    obj->put(L"name", name);
    obj->put(L"initial", initializer->json());

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
    obj->put(L"mutability", WideString{ty});
    return obj;
  }
}
