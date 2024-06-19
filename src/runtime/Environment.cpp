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
#include "token/Array.hpp"

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
    static auto global = crab::make_rc_mut<Environment>(RefMut{intepreter});
    static bool initialised = false;

    if (initialised) return global;
    initialised = true;

    // Math
    {
      const RcMut<Dictionary> math = crab::make_rc_mut<Dictionary>();
      global->define_constant("math", math);
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
      const RcMut<Dictionary> str = crab::make_rc_mut<Dictionary>();
      global->define_constant("str", str);
      str->set("empty", WideString{});
      str->set_func(
        "parse_number",
        [](const WideString &string) {
          f64 num;
          WideStringStream{string} >> num;
          return num;
        }
      );
      // const RcMut<Dictionary> fmt = crab::make_rc_mut<Dictionary>();
      // env->define_constant("fmt", fmt);
    }

    global->define_builtin(
      L"typeof",
      [](const IValue &obj) {
        return str::convert((StringStream{} << obj.get_type()).str());
      }
    );

    global->define_builtin(
      L"clone",
      [](const IValue &obj) { return obj.clone(); }
    );

    // ReSharper disable once CppPassValueParameterByConstReference
    global->define_builtin(
      L"import",
      [](const WideString &file) {
        SourceFile source = SourceFile::from_file(file + L".goo").take_unchecked();
        lexer::TokenList list{lexer::Lexer::tokenize(std::move(source)).take_unchecked()};

        if (list.empty()) {
          return ok(Unit::value());
        }
        parser::TokenStream stream{std::move(list)};
        return Intepreter{}.evaluate(parser::pass::block_top_level(stream).take_unchecked());
      }
    );

    global->define_builtin(
      L"include_str",
      [](const WideString &path)-> WideString {
        std::wifstream stream{str::convert(path)};

        if (stream.bad()) return L"";

        stream.imbue(std::locale(std::locale{}, new std::codecvt_utf8<widechar>));
        WideStringStream collector{};
        collector << stream.rdbuf();
        return collector.str();
      }
    );

    global->define_builtin(
      L"fmt",
      [](const Intepreter &runtime, const WideString &fmt_string) {
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

          os << runtime.env()
                       .get_variable(meta::Identifier::from(name.str()))
                       .take_unchecked()
                       ->get_value()->
                       to_string();
        }

        return std::move(os).str();
      }
    );

    global->define_builtin(
      L"len",
      [](IValue &any) -> usize {
        if (auto dict = any.coerce<Dictionary>()) {
          return dict.take_unchecked()->length();
        }

        if (auto arr = any.coerce<Array>()) {
          return arr.take_unchecked()->length();
        }

        if (auto str = any.coerce<GString>()) {
          return str.take_unchecked()->get()->length();
        }

        return 0;
      }
    );

    global->define_builtin(
      L"each",
      [](Intepreter &runtime, IValue &any, const ICallable &func) -> Result<Any> {
        if (auto arr = any.coerce<Array>()) {
          for (const auto &v: arr.get_unchecked()->get()) {
            if (auto result = func.call(runtime, {v}); result.is_err())
              return result.take_err_unchecked();
          }
        }

        if (auto arr = any.coerce<Dictionary>()) {
          for (const auto &pair: arr.get_unchecked()->get_pairs() | std::views::values) {
            auto [key, value] = pair;
            if (auto result = func.call(runtime, {key, value}); result.is_err())
              return result.take_err_unchecked();
          }
        }
        return ok(Unit::value());
      }
    );

    global->define_builtin(
      L"fatal",
      [](const i32 x) {
        std::wcerr << std::endl << "skull emogi lol" << std::endl;
        std::exit(x);
      }
    );

    global->define_builtin(
      L"push",
      [](const RcMut<Array> &arr, Any value) {
        arr->push(std::move(value));
      }
    );

    global->define_builtin(
      L"idx",
      [](const Array &arr, const usize i) {
        if (i >= arr.length()) return Any{Unit::value()};
        return arr.get().at(i);
      }
    );
    global->define_builtin(
      L"get",
      [](const Dictionary &dict, const GString &str) {
        if (auto v = dict.get(str)) {
          return v.take_unchecked();
        }
        return Any{Unit::value()};
      }
    );

    global->define_builtin(
      L"readline",
      [] {
        WideString str;
        std::getline(std::wcin, str);
        return str;
      }
    );

    global->define_builtin(
      L"print",
      ExternFunction::varargs(
        [](Intepreter &, const Vec<Any> &args) -> Result<Any> {
          for (const auto &arg: args) {
            std::wcout << arg->to_string();
          }
          std::wcout << std::endl;
          return crab::ok<Any>(crab::make_rc_mut<Unit>());
        }
      )
    );

    return global;
  }

  auto Environment::top_level_enviornment(Intepreter &intepreter) -> RcMut<Environment> {
    return crab::make_rc_mut<Environment>(RefMut{intepreter}, crab::none);
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

  auto Environment::runtime() const -> Intepreter& {
    return intepreter;
  }
}
