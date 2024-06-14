//
// Created by bishan_ on 5/15/24.
//

#include "Environment.hpp"

#include <utility>

#include "data/Value.hpp"

namespace goos::runtime {
  Variable::Variable(const meta::Mutability mutability, RcMut<Value> value)
    : mutability{mutability},
      value{std::move(value)},
      type{this->value->get_type()} {}

  auto Variable::get_mutability() const -> meta::Mutability {
    return mutability;
  }

  auto Variable::get_value() const -> RcMut<Value> {
    return value;
  }

  auto Variable::get_type() const -> meta::VariantType {
    return type;
  }

  auto Variable::set_value(RcMut<Value> value) -> void {
    this->value = std::move(value);
  }

  Environment::Environment(RcMut<Environment> parent) : Environment{crab::some(std::move(parent))} {}

  Environment::Environment(Option<RcMut<Environment>> parent)
    : parent{std::move(parent)} {
    if (auto p = this->parent) {
      depth = p.get_unchecked()->depth + 1;
    }
  }

  auto Environment::push_variable(
    meta::Identifier identifier,
    const meta::Mutability mutability,
    RcMut<Value> value
  ) -> void {
    // TODO error handling for scope
    bindings.emplace(std::move(identifier), crab::make_rc_mut<Variable>(mutability, std::move(value)));
  }

  // ReSharper disable once CppMemberFunctionMayBeConst
  auto Environment::set_value // NOLINT(*-make-member-function-const)
  (const meta::Identifier &identifier, RcMut<Value> value) -> void {
    get_variable(identifier)->set_value(std::move(value));
  }

  auto Environment::get_variable(const meta::Identifier &identifier) const -> RcMut<Variable> {
    // TODO error handling
    return try_get_variable(identifier).take_unchecked();
  }

  auto Environment::try_get_variable(const meta::Identifier &identifier) const -> Option<RcMut<Variable>> {
    if (bindings.contains(identifier)) {
      return bindings.at(identifier);
    }

    if (parent.is_some()) {
      return parent.get_unchecked()->try_get_variable(identifier);
    }

    return crab::none;
  }

  auto Environment::get_depth() const -> usize {
    return depth;
  }

  auto Environment::get_previous() const -> Option<RcMut<Environment>> {
    return parent;
  }
}
