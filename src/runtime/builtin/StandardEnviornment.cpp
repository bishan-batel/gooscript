//
// Created by bishan_ on 6/19/24.
//

#include "StandardEnviornment.hpp"

#include <cmath>
#include <fstream>

#include "runtime/data/ExternFunction.hpp"
#include "runtime/Intepreter.hpp"
#include "runtime/data/Dictionary.hpp"
#include "runtime/data/TypeConversion.hpp"
#include "runtime/data/Unit.hpp"
#include "runtime/err/DuplicateName.hpp"
#include <ref.hpp>

#include "runtime/data/interfaces/ICallable.hpp"
#include "parser/TokenStream.hpp"
#include "parser/pass/statement/block.hpp"
#include "../data/Array.hpp"

namespace goos::runtime::builtin {
  static auto BUILTIN = crab::make_rc_mut<Environment>();

  auto math_module() {
    const RcMut<Dictionary> module = crab::make_rc_mut<Dictionary>();
    BUILTIN->define_constant("math", module);
    Dictionary &mod = *module;

    using namespace std;
    mod.set("PI", numbers::pi);
    mod.set("π", numbers::pi);
    mod.set("E", numbers::e);
    mod.set("SQRT_2", numbers::sqrt2);
    mod.set("PHI", numbers::phi);

    #define lambda(args, expr) []args{ return expr; }
    #define func(name, args, expr) mod.set_func(#name, lambda(args, expr))

    using number = const INumeric&;

    func(min, (number a, number b), min(*a, *b));
    func(max, (number a, number b), min(*a, *b));
    func(abs, (number a), abs(*a));
    func(sqrt, (number a), sqrt(*a));
    func(acos, (number a), acos(*a));
    func(asin, (number a), asin(*a));
    func(atan, (number a), atan(*a));
    func(cbrt, (number a), cbrt(*a));
    func(ceil, (number a), ceil(*a));
    func(cos, (number a), cos(*a));
    func(sin, (number a), sin(*a));
    func(floor, (number a), floor(*a));
    func(log, (number a), log(*a));
    func(round, (number a), round(*a));
    func(hypot, (number a, number b), hypot(*a, *b));

    #undef func
    #undef lambda
  }

  auto str_module() {
    const RcMut<Dictionary> module = crab::make_rc_mut<Dictionary>();
    BUILTIN->define_constant("str", module);
    Dictionary &mod = *module;

    mod.set("empty", L"");
    mod.set("newline", L"\n");
    mod.set("quote", L"\"");

    mod.set_func(
      "parse_number",
      [](const WideString &string) {
        f64 num;
        WideStringStream{string} >> num;
        return num;
      }
    );

    mod.set_func(
      "is_digit",
      [](const WideString &str) {
        if (str.empty()) return false;

        const widechar c = str.at(0);
        return c >= L'0' and c <= L'9';
      }
    );

    mod.set_func(
      "contains",
      [](const WideString &str, const WideString &c) {
        return str.contains(c);
      }
    );

    mod.set_func(
      "contains_any_char",
      [](const WideString &str, const WideString &match) {
        return std::ranges::any_of(match, [&](const widechar c) { return str.contains(c); });
      }
    );
  }

  auto get_standard_enviornment() -> RcMut<Environment> {
    static bool initialised = false;

    if (initialised) return BUILTIN;
    initialised = true;

    math_module();
    str_module();

    BUILTIN->define_builtin(
      L"typeof",
      [](const IValue &obj) {
        return str::convert((StringStream{} << obj.get_type()).str());
      }
    );

    BUILTIN->define_builtin(
      L"clone",
      [](const IValue &obj) { return obj.clone(); }
    );

    // ReSharper disable once CppPassValueParameterByConstReference
    BUILTIN->define_builtin(
      L"import",
      [](const WideString &file) {
        SourceFile source = SourceFile::from_file(file).take_unchecked();
        lexer::TokenList list{lexer::Lexer::tokenize(std::move(source)).take_unchecked()};

        if (list.empty()) {
          return ok(Unit::value());
        }
        parser::TokenStream stream{std::move(list)};
        return Intepreter{}.evaluate(parser::pass::block_top_level(stream).take_unchecked());
      }
    );

    BUILTIN->define_builtin(
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

    BUILTIN->define_builtin(
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

    BUILTIN->define_builtin(
      L"len",
      [](IValue &any) -> usize {
        switch (any.get_type()) {
          case meta::VariantType::OBJECT:
            return any.coerce_unchecked<Dictionary>().length();
          case meta::VariantType::STRING:
            return any.coerce_unchecked<GString>().get()->length();
          case meta::VariantType::ARRAY:
            return any.coerce_unchecked<Array>().length();
          default:
            return 0;
        }
      }
    );

    BUILTIN->define_builtin(
      L"each",
      [](Intepreter &runtime, IValue &any, const ICallable &func) -> Result<Any> {
        switch (any.get_type()) {
          case meta::VariantType::ARRAY: {
            const auto &arr = any.coerce_unchecked<Array>();
            for (const auto &v: arr.get()) {
              if (auto result = func.call(runtime, {v}); result.is_err())
                return result.take_err_unchecked();
            }
            break;
          }

          case meta::VariantType::OBJECT: {
            for (const auto &[key, value]:
                 any.coerce_unchecked<Dictionary>().get_pairs() | std::views::values
            ) {
              if (auto result = func.call(runtime, {key, value}); result.is_err())
                return result.take_err_unchecked();
            }
            break;
          }

          case meta::VariantType::STRING: {
            for (const widechar c:
                 *any.coerce_unchecked<GString>().get()
            ) {
              if (auto result = func.call(runtime, {crab::make_rc_mut<GString>(WideString{c})}); result.is_err())
                return result.take_err_unchecked();
            }
            break;
          }

          default:
            return ok(Nil::value());
        }
        return ok(Unit::value());
      }
    );

    BUILTIN->define_builtin(
      L"fatal",
      [](const i32 x) {
        std::wcerr << std::endl << "skull emogi lol" << std::endl;
        std::exit(x);
      }
    );

    BUILTIN->define_builtin(
      L"push",
      [](const RcMut<Array> &arr, Any value) {
        arr->push(std::move(value));
      }
    );

    BUILTIN->define_builtin(
      L"idx",
      [](const Array &arr, const usize i) {
        if (i >= arr.length()) return Any{Unit::value()};
        return arr.get().at(i);
      }
    );
    BUILTIN->define_builtin(
      L"get",
      [](const Dictionary &dict, const GString &str) {
        if (auto v = dict.get(str)) {
          return v.take_unchecked();
        }
        return Any{Unit::value()};
      }
    );

    BUILTIN->define_builtin(
      L"readline",
      [] {
        WideString str;
        std::getline(std::wcin, str);
        return str;
      }
    );

    BUILTIN->define_builtin(
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

    return BUILTIN;
  }
}
