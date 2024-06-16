//
// Created by bishan_ on 5/15/24.
//

#include "Environment.hpp"

#include <utility>

#include "data/ExternFunction.hpp"
#include "Intepreter.hpp"
#include "data/TypeConversion.hpp"
#include "data/Unit.hpp"
#include "data/IValue.hpp"
#include "err/DuplicateName.hpp"
#include "err/VariableNotFound.hpp"

namespace goos::runtime {
  Variable::Variable(const meta::Mutability mutability, RcMut<IValue> value)
    : mutability{mutability},
      value{std::move(value)},
      type{this->value->get_type()} {}

  auto Variable::get_mutability() const -> meta::Mutability {
    return mutability;
  }

  auto Variable::get_value() const -> RcMut<IValue> {
    return value;
  }

  auto Variable::get_type() const -> meta::VariantType {
    return type;
  }

  auto Variable::set_value(RcMut<IValue> value) -> void {
    this->value = std::move(value);
  }

  auto Environment::define_builtin(WideString name, const RcMut<ExternFunction> &function) -> void {
    push_variable(
      meta::Identifier{std::move(name)},
      meta::Mutability::IMMUTABLE,
      function
    );
  }

  auto Environment::get_standard_environment(Intepreter &intepreter) -> RcMut<Environment> {
    auto env = crab::make_rc_mut<Environment>(RefMut{intepreter});

    env->define_constant("PI", type::to_goos(std::numbers::pi));

    // env->push_variable(meta::Identifier{L"true"}, meta::Mutability::IMMUTABLE, crab::make_rc_mut<Boolean>(true));
    // env->push_variable(meta::Identifier{L"false"}, meta::Mutability::IMMUTABLE, crab::make_rc_mut<Boolean>(false));

    env->define_builtin(
      L"funny_number",
      ExternFunction::from(
        0,
        [](Environment &, const Vec<Any> &) -> Result<Any> {
          return crab::ok(type::to_goos_any(42069LL));
        }
      )
    );

    env->define_builtin(
      L"log",
      ExternFunction::from(
        1,
        [](Environment &, const Vec<Any> &args) -> Result<Any> {
          const auto v = type::try_from_goose<i64>(args.at(0));

          std::cout << "[builtin:log] " << v << std::endl;

          return ok(Unit::value());
        }
      )
    );

    env->define_builtin(
      L"log",
      [](const IValue &first) {
        std::wcout << "[builtin:log2] " << first.to_string() << std::endl;
        return ok(Unit::value());
      }
    );

    env->define_builtin(
      L"min",
      [](const i64 a, const i64 b) {
        return ok(crab::make_rc_mut<Integer>(std::min(a, b)));
      }
    );

    //
    env->define_builtin(
      L"readline",
      ExternFunction::from(
        0,
        [](Environment &, const Vec<Any> &) -> Result<Any> {
          WideString str;
          std::getline(std::wcin, str);
          return crab::ok(type::to_goos_any(std::move(str)));
        }
      )
    );

    env->define_builtin(
      L"print",
      ExternFunction::varargs(
        [](Environment &, const Vec<Any> &args) -> Result<Any> {
          for (const auto &arg: args) {
            std::wcout << arg->to_string();
          }
          std::wcout << std::endl;
          return crab::ok<Any>(crab::make_rc_mut<Unit>());
        }
      )
    );

    return env;
  }

  Environment::Environment(Intepreter &intepreter, RcMut<Environment> parent)
    : Environment{intepreter, crab::some(std::move(parent))} {}

  Environment::Environment(Intepreter &intepreter, Option<RcMut<Environment>> parent)
    : parent{std::move(parent)}, intepreter{intepreter} {
    if (auto p = this->parent) {
      depth = p.get_unchecked()->depth + 1;
    }
  }

  auto Environment::enclose(RcMut<Environment> enclosing) -> RcMut<Environment> {
    return crab::make_rc_mut<Environment>(enclosing->intepreter, std::move(enclosing));
  }

  auto Environment::push_variable(
    meta::Identifier identifier,
    const meta::Mutability mutability,
    RcMut<IValue> value
  ) -> Result<RcMut<Variable>> {
    if (get_variable(identifier).is_ok()) {
      return err::make<err::DuplicateName>(identifier);
    }

    bindings.emplace(std::move(identifier), crab::make_rc_mut<Variable>(mutability, std::move(value)));

    return get_variable(identifier);
  }

  auto Environment::set_value(
    const meta::Identifier &identifier,
    RcMut<IValue> value
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

  auto Environment::runtime() const -> Intepreter& {
    return intepreter;
  }
}
