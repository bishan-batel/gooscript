//
// Created by bishan_ on 5/15/24.
//

#include "Environment.hpp"

#include <cmath>
#include <fstream>
#include <utility>

#include "data/ExternFunction.hpp"
#include "Intepreter.hpp"
#include "data/Dictionary.hpp"
#include "data/TypeConversion.hpp"
#include "data/Unit.hpp"
#include "data/interfaces/IValue.hpp"
#include "err/DuplicateName.hpp"
#include "err/VariableNotFound.hpp"
#include <ref.hpp>

#include <fmt/xchar.h>

#include "data/interfaces/ICallable.hpp"
#include "parser/TokenStream.hpp"
#include "parser/pass/statement/block.hpp"
#include "data/Array.hpp"

namespace goos::runtime {
  Variable::Variable(const meta::Mutability mutability, Any value)
    : mutability{mutability},
      value{std::move(value)},
      type{this->value->get_type()} {}

  auto Variable::get_mutability() const -> meta::Mutability {
    return mutability;
  }

  auto Variable::get_value() const -> Any {
    return value;
  }

  auto Variable::get_type() const -> meta::VariantType {
    return type;
  }

  auto Variable::as_lvalue() -> RcMut<LValue> {
    return LValue::wrap(value);
  }

  auto Variable::set_value(Any value) -> void {
    this->value = std::move(value);
  }

  auto Environment::define_builtin(WideString name, const RcMut<ExternFunction> &function) -> void {
    push_variable(
      meta::Identifier::from(std::move(name)),
      meta::Mutability::IMMUTABLE,
      function
    );
  }

  auto Environment::top_level_enviornment() -> RcMut<Environment> {
    return crab::make_rc_mut<Environment>(crab::none);
  }

  Environment::Environment(RcMut<Environment> parent)
    : Environment{crab::some(std::move(parent))} {}

  Environment::Environment(Option<RcMut<Environment>> parent)
    : parent{std::move(parent)} {
    if (auto p = this->parent) {
      depth = p.get_unchecked()->depth + 1;
    }
  }

  auto Environment::enclose(RcMut<Environment> enclosing) -> RcMut<Environment> {
    return crab::make_rc_mut<Environment>(std::move(enclosing));
  }

  auto Environment::push_variable(
    meta::Identifier identifier,
    const meta::Mutability mutability,
    Any value
  ) -> void {
    RcMut<Variable> var = crab::make_rc_mut<Variable>(mutability, std::move(value));

    if (get_variable(identifier).is_ok()) {
      bindings.emplace(std::move(identifier), var);
      return;
    }

    bindings.emplace(std::move(identifier), var);
  }

  auto Environment::set_value(
    const meta::Identifier &identifier,
    Any value
  ) const -> Result<RcMut<Variable>> {
    Result<RcMut<Variable>> var = get_variable(identifier);

    if (var.is_ok()) {
      var.get_unchecked()->set_value(std::move(value));
    }

    return var;
  }

  auto Environment::get_variable(const meta::Identifier &identifier) const -> Result<RcMut<Variable>> {
    // TODO error handling
    // return try_get_variable(identifier).take_unchecked();
    if (bindings.contains(identifier)) {
      return crab::ok(bindings.at(identifier));
    }

    if (parent.is_some()) {
      return parent.get_unchecked()->get_variable(identifier);
    }

    return err::make<err::VariableNotFound>(identifier);
  }

  auto Environment::get_depth() const -> usize {
    return depth;
  }

  auto Environment::get_previous() const -> Option<RcMut<Environment>> {
    return parent;
  }
}
