//
// Created by bishan_ on 6/21/24.
//

#pragma once
#include <box.hpp>
#include <preamble.hpp>
#include <variant>

#include "err/Error.hpp"
#include "ast/expression/literal/Array.hpp"
#include "err/CastError.hpp"

namespace goos::vm {
  struct Object {};

  struct Nil {
    const static Nil val;

    constexpr Nil() = default;

    [[nodiscard]] constexpr auto operator==(const Nil &) const -> bool { return true; }
  };

  inline const Nil Nil::val{};

  template<typename T>
  concept numeric = std::same_as<T, f64> or std::same_as<T, i64>;

  class Value final {
    using Variant = std::variant<i64, f64, bool, Nil, unit>;
    Variant value;

  public:
    template<typename>
    struct type_name_of {};

    template<typename Type>
    static constexpr StringView type_name_of_s = type_name_of<Type>::name;

    // ReSharper disable once CppNonExplicitConvertingConstructor
    template<typename T>
    constexpr Value(T val) : value{std::move(val)} {}; // NOLINT(*-explicit-constructor)

    // ReSharper disable once CppNonExplicitConvertingConstructor
    constexpr Value(unit) : value{unit::val} {}; // NOLINT(*-explicit-constructor)

    template<typename T>
    [[nodiscard]] constexpr auto as() const -> Result<T>;

    template<typename T, typename... Others>
    [[nodiscard]] constexpr auto is() const -> bool;

    [[nodiscard]] constexpr auto as_number() const -> f64;

    [[nodiscard]] constexpr auto is_numeric() const -> bool;

    [[nodiscard]] constexpr auto get_value() const -> const Variant&;

    [[nodiscard]] constexpr auto type_name() const -> StringView;

    [[nodiscard]] auto operator==(const Value &other) const -> bool;

    template<typename... Types>
    [[nodiscard]] constexpr auto match(Types... match) const {
      return this->match(crab::cases{match...});
    }

    template<typename... Types>
    [[nodiscard]] constexpr auto match(Types... match) {
      return this->match(crab::cases{match...});
    }

    template<typename Visitor, std::same_as<Value>... Variants>
    [[nodiscard]] constexpr auto match(Visitor overload, Variants... others) const {
      return std::visit(overload, value, others.value...);
    }

    template<typename Visitor, std::same_as<Value>... Variants>
    [[nodiscard]] constexpr auto match(Visitor overload, Variants... others) {
      return std::visit(overload, value, others.value...);
    }

    [[nodiscard]] constexpr auto variant() const -> const Variant& { return value; }

    [[nodiscard]] constexpr auto variant() -> Variant& { return value; }

    #define type_name_of(ty) template<> struct type_name_of<ty> { constexpr static StringView name{#ty}; }

    type_name_of(i64);

    type_name_of(f64);

    type_name_of(bool);

    type_name_of(Nil);

    type_name_of(unit);

    #undef type_name_of
  };

  constexpr auto Value::as_number() const -> f64 {
    return match(
      [](const f64 a) { return a; },
      [](const i64 b) { return static_cast<f64>(b); },
      [](const auto &) -> f64 { return 0.; }
    );
  }

  constexpr auto Value::is_numeric() const -> bool {
    return is<f64, i64>();
  }

  constexpr auto Value::get_value() const -> const Variant& { return value; }

  constexpr auto Value::type_name() const -> StringView {
    using namespace std::literals;
    return match([]<typename T>(const T &) { return type_name_of_s<T>; });
  }

  template<typename T>
  constexpr auto Value::as() const -> Result<T> {
    if (not is<T>()) {
      return make_err<err::CastError>(type_name(), type_name_of_s<T>);
    }
    return std::get<T>(value);
  }

  template<typename T, typename... Others>
  constexpr auto Value::is() const -> bool { return std::holds_alternative<T, Others...>(value); }
}
