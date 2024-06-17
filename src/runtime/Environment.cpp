//
// Created by bishan_ on 5/15/24.
//

#include "Environment.hpp"

#include <cmath>
#include <utility>

#include "data/ExternFunction.hpp"
#include "Intepreter.hpp"
#include "data/Dictionary.hpp"
#include "data/TypeConversion.hpp"
#include "data/Unit.hpp"
#include "data/IValue.hpp"
#include "err/DuplicateName.hpp"
#include "err/VariableNotFound.hpp"
#include <ref.hpp>

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

  auto Environment::get_standard_environment(Intepreter &intepreter) -> RcMut<Environment> {
    static auto env = crab::make_rc_mut<Environment>(RefMut{intepreter});
    static bool initialised = false;

    if (initialised) return env;
    initialised = true;

    // Math
    {
      const RcMut<Dictionary> math = crab::make_rc_mut<Dictionary>();
      env->define_constant("math", math);
      math->set("PI", std::numbers::pi);
      math->set("π", std::numbers::pi);

      math->set_func("min", [](const f64 a, const f64 b) { return std::min(a, b); });
      math->set_func("min", [](const i64 a, const i64 b) { return std::min(a, b); });
      math->set_func("max", [](const f64 a, const f64 b) { return std::min(a, b); });
      math->set_func("max", [](const i64 a, const i64 b) { return std::max(a, b); });
      math->set_func("abs", [](const i64 a) { return std::abs(a); });
      math->set_func("abs", [](const f64 a) { return std::abs(a); });
      math->set_func("sqrt", [](const f64 a) { return std::sqrt(a); });
      math->set_func("acos", [](const f64 a) { return std::acos(a); });
      math->set_func("asin", [](const f64 a) { return std::asin(a); });
      math->set_func("atan", [](const f64 a) { return std::atan(a); });
      math->set_func("cbrt", [](const f64 a) { return std::cbrt(a); });
      math->set_func("ceil", [](const f64 a) { return std::ceil(a); });
      math->set_func("cos", [](const f64 a) { return std::cos(a); });
      math->set_func("sin", [](const f64 a) { return std::sin(a); });
      math->set_func("floor", [](const f64 a) { return std::floor(a); });
      math->set_func("log", [](const f64 a) { return std::log(a); });
      math->set_func("round", [](const f64 a) { return std::round(a); });
      math->set_func("hypot", [](const f64 a, const f64 b) { return std::hypot(a, b); });
    } {
      // const RcMut<Dictionary> fmt = crab::make_rc_mut<Dictionary>();
      // env->define_constant("fmt", fmt);
    }

    env->define_builtin(
      L"fmt",
      ExternFunction::from(
        1,
        [](const Environment &inner_env, const Vec<Any> &args) -> Result<Any> {
          const WideString &fmt_string = args.at(0)->coerce_unchecked<GString>().get().get();

          const usize len = fmt_string.length();

          WideStringStream os{};

          for (usize i = 0; i < len; i++) {
            if (const widechar c = fmt_string.at(i); c != '{') {
              os << c;
              continue;
            }

            i++;
            if (i < len and fmt_string.at(i) == '{') {
              continue;
            }

            WideStringStream name{};
            for (usize j = i; j < len; j++) {
              if (fmt_string.at(j) == '}') {
                break;
              }
              name << fmt_string.at(j);
              i++;
            }

            os << inner_env.get_variable(meta::Identifier::from(name.str())).take_unchecked()->get_value()->to_string();
          }

          return ok(crab::make_rc_mut<GString>(std::move(os).str()));
        }
      )
    );

    env->define_builtin(
      L"💀",
      [] {
        std::wcerr << std::endl << "skull emogi lol" << std::endl;
        std::exit(69);
      }
    );

    env->define_builtin(
      L"readline",
      [] {
        WideString str;
        std::getline(std::wcin, str);
        return str;
      }
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
    Any value
  ) -> Result<RcMut<Variable>> {
    if (get_variable(identifier).is_ok()) {
      return err::make<err::DuplicateName>(identifier);
    }

    bindings.emplace(std::move(identifier), crab::make_rc_mut<Variable>(mutability, std::move(value)));

    return get_variable(identifier);
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

  auto Environment::runtime() const -> Intepreter& {
    return intepreter;
  }
}
