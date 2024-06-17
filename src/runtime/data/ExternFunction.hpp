//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include <functional>

#include <option.hpp>
#include "ICallable.hpp"
#include "IValue.hpp"
#include "runtime/err/InvalidCast.hpp"
#include "runtime/err/NotCallable.hpp"

namespace goos::runtime {
  class ExternFunction final : public ICallable {
    std::function<Result<Any>(Environment &env, const Vec<Any> &args)> function;
    usize arity{};
    ArityType arity_type;

    explicit ExternFunction(decltype(function) function, usize arity, ArityType arity_type);

  public:
    static constexpr meta::VariantType TYPE{meta::VariantType::FUNCTION};

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
    template<typename>
    struct transmute_argument {
      static constexpr meta::VariantType into_type
          = meta::VariantType::ANY;
    };

    // template<typename Into> requires (not type::convertible_primitive<Into>)
    // struct transmute_argument<Into> {
    //   auto operator()(const Any &) const -> Option<Into> {
    //     return crab::none;
    //   }
    // };

    template<>
    struct transmute_argument<Any> {
      static constexpr meta::VariantType into_type
          = meta::VariantType::ANY;

      auto operator()(Any any) const -> Option<Any> {
        return Option<Any>{std::move(any)};
      }
    };

    template<type::specialized_value T>
    struct transmute_argument<RcMut<T>> {
      static constexpr meta::VariantType into_type
          = T::TYPE;

      auto operator()(Any any) const -> Option<RcMut<T>> {
        return any.downcast<T>();
      }
    };

    template<type::value_type T>
    struct transmute_argument<Ref<T>> {
      static constexpr meta::VariantType into_type
          = T::TYPE;

      auto operator()(Any any) const -> Option<Ref<T>> {
        Option<RcMut<T>> casted = any.downcast<T>();
        if (casted.is_none()) return crab::none;
        return crab::some(Ref{*casted.take_unchecked()});
      }
    };

    template<type::value_type T>
    struct transmute_argument<RefMut<T>> {
      static constexpr meta::VariantType into_type
          = T::TYPE;

      auto operator()(Any any) const -> Option<RefMut<T>> {
        Option<RcMut<T>> casted = any.downcast<T>();
        if (casted.is_none()) return crab::none;
        return crab::some(RefMut{*casted.take_unchecked()});
      }
    };

    template<>
    struct transmute_argument<Ref<Any>> {
      static constexpr meta::VariantType into_type
          = meta::VariantType::ANY;

      auto operator()(Any any) const -> Option<Any> {
        return Option{std::move(any)};
      }
    };

    template<type::convertible_primitive Primitive>
    struct transmute_argument<Primitive> {
      static constexpr meta::VariantType into_type
          = type::primitive_to_goose_t<Primitive>::TYPE;

      auto operator()(const Any &any) const -> Option<Primitive> {
        return type::try_from_goose<Primitive>(any);
      }
    };

    template<typename T>
    struct transmute_argument<Option<T>> {
      static constexpr meta::VariantType into_type = transmute_argument<T>::into_type;

      auto operator()(const Any &any) const -> Option<Option<T>> {
        return Option{transmute_argument<T>{}(any)};
      }
    };

    template<typename>
    struct transmute_return {};

    // Identity Case
    template<>
    struct transmute_return<Result<Any>> {
      auto operator()(Result<Any> result) const -> Result<Any> {
        return result;
      }
    };

    template<type::specialized_value T>
    struct transmute_return<RcMut<T>> {
      auto operator()(RcMut<T> returned) const -> Result<Any> {
        return Result<Any>{Any{returned}};
      }
    };

    template<type::specialized_value T>
    struct transmute_return<Result<RcMut<T>>> {
      auto operator()(Result<RcMut<T>> returned) const -> Result<Any> {
        if (returned.is_err()) {
          return returned.take_err_unchecked();
        }
        return Result<Any>{Any{returned.take_unchecked()}};
      }
    };

    template<type::convertible_primitive T>
    struct transmute_return<T> {
      auto operator()(T returned) const -> Result<Any> {
        return Result<Any>{type::to_goos_any(returned)};
      }
    };

    template<type::convertible_primitive T>
    struct transmute_return<Result<T>> {
      auto operator()(Result<T> returned) const -> Result<Any> {
        if (returned.is_err()) return returned.take_err_unchecked();
        return Result<Any>{type::to_goos_any(returned)};
      }
    };

    template<typename Into>
    auto opt_to_err(const Vec<Any> &args, const usize index) -> Option<Box<err::Error>> {
      using transmute_t = transmute_argument<typename crab::ref::decay_type<Into>::type>;

      constexpr transmute_t transmute{};

      if (index >= args.size()) {
        return err::make<err::InvalidCast>(meta::VariantType::NIL, transmute_t::into_type).value;
      }

      const Any &arg = args[index];

      if (transmute(arg).is_some()) {
        return crab::none;
      }

      return err::make<err::InvalidCast>(arg->get_type(), transmute_t::into_type).value;
    }

    template<typename T>
    auto fold_option() -> Option<T> { return crab::none; }

    template<typename T>
    auto fold_option(Option<T> first) -> Option<T> { return first; }

    template<typename T, typename... R>
    auto fold_option(Option<T> first, Option<R>... args) -> Option<T> {
      if (first.is_some()) {
        return first;
      }

      return lambda::fold_option<R...>(std::move(args)...);
    }

    //
    template<typename Output, typename... Parameters>
    static auto from_function(const std::function<Output(Parameters...)> &function) -> RcMut<ExternFunction> {
      constexpr usize arity = sizeof...(Parameters);

      return ExternFunction::from(
        arity,
        [function](Environment &, const Vec<Any> &args) -> Result<Any> {
          usize i = 0;
          auto next = [&i] { return i++; };

          if (auto typed_args = lambda::fold_option<Box<err::Error>>(
            lambda::opt_to_err<Parameters>(args, next())...
          ); typed_args.is_some()) {
            return crab::err(typed_args.take_unchecked());
          }

          i = 0;

          if constexpr (not std::is_same_v<Output, void>) {
            // TODO type checking
            return transmute_return<Output>{}(
              function(
                transmute_argument<typename crab::ref::decay_type<Parameters>::type>{}(args.at(next())).take_unchecked()
                ...
              )
            );
          }
          return ok(Unit::value());
        }
      );
    }

    template<typename F>
    static auto from(const F function) -> RcMut<ExternFunction> {
      return lambda::from_function(std::function{function});
    }
  };

  // return
  // type::from_goos (any);
}
