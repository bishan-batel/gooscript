//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include <functional>

#include <option.hpp>
#include "ICallable.hpp"
#include "IValue.hpp"

namespace goos::runtime {
  class ExternFunction final : public ICallable {
    std::function<Result<Any>(Environment &env, const Vec<Any> &args)> function;
    usize arity{};
    ArityType arity_type;

    explicit ExternFunction(decltype(function) function, usize arity, ArityType arity_type);

  public:
    static auto from(usize arity, decltype(function) function) -> RcMut<ExternFunction>;

    static auto varargs(const decltype(function) &function) -> RcMut<ExternFunction>;

    [[nodiscard]] auto call(Environment &env, const Vec<Any> &values) const -> Result<Any> override;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    [[nodiscard]] auto get_arity_type() const -> ArityType override;

    [[nodiscard]] auto get_arity() const -> usize override;

    [[nodiscard]] auto base_hash() const -> usize override;

    [[nodiscard]] auto clone() const -> Any override;
  };

  // template<typename... Parameters>
  // using ExternLambdaFunction = std::function<Result<Any>(Environment &, Parameters...)>;

  // template<typename T>
  // auto lamba_get_argument(T arg) -> T {
  //   if constexpr (std::is_same_v<T, Environment&>) {
  //     if (i == 0) {
  //       return env;
  //     }
  //   }
  //
  //   return type::try_from_goose<T>(args[++i]).take_unchecked();
  // }

  namespace lambda {
    template<typename T>
    struct into_crab {
      using type = T;
    };

    template<typename T>
    struct into_crab<T&> {
      using type = RefMut<T>;
    };

    template<typename T>
    struct into_crab<const T&> {
      using type = Ref<T>;
    };

    template<typename Into>
    auto transmute_argument(Any any) -> Option<Into> {}

    template<>
    inline auto transmute_argument(Any any) -> Option<Any> { return any; }

    template<typename T> requires type::is_concrete_value<T>
    auto transmute_argument(Any any) -> Option<RcMut<T>> { return any.downcast<T>(); }

    template<typename Primitive> requires type::is_convertible_primitive<Primitive>
    auto transmute_argument(const Any any) -> Option<Primitive> { return type::try_from_goose<Primitive>(any); }

    template<typename... Parameters>
    static auto from_function(const std::function<Result<Any>(Parameters...)> &function) -> RcMut<ExternFunction> {
      constexpr usize arity = sizeof...(Parameters);

      return ExternFunction::from(
        arity,
        [function](Environment &, const Vec<Any> &args) -> Result<Any> {
          usize i = 0;
          auto next = [&i] { return i++; };

          // TODO type checking
          return function(
            (transmute_argument<typename into_crab<Parameters>::type>(args.at(next())).take_unchecked())...
          );
        }
      );
    }

    template<typename F>
    static auto from(const F function) -> RcMut<ExternFunction> {
      return from_function(std::function{function});
    }
  };

  // return
  // type::from_goos (any);
}
