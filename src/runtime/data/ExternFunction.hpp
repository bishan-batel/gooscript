//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include <functional>

#include <option.hpp>
#include "interfaces/IValue.hpp"
#include "interfaces/ICallable.hpp"
#include "runtime/Intepreter.hpp"
#include "runtime/err/InvalidCast.hpp"

namespace goos::runtime {
  class ExternFunction final : public ICallable {
    std::function<Result<Any>(Intepreter &runtime, const Vec<Any> &args)> function;
    usize arity{};
    ArityType arity_type;

    explicit ExternFunction(decltype(function) function, usize arity, ArityType arity_type);

  public:
    static constexpr meta::VariantType TYPE{meta::VariantType::FUNCTION};

    static auto from(usize arity, decltype(function) function) -> RcMut<ExternFunction>;

    static auto varargs(const decltype(function) &function) -> RcMut<ExternFunction>;

    [[nodiscard]] auto call(Intepreter &runtime, const Vec<Any> &values) const -> Result<Any> override;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    [[nodiscard]] auto get_arity_type() const -> ArityType override;

    [[nodiscard]] auto get_arity() const -> usize override;

    [[nodiscard]] auto base_hash() const -> usize override;

    [[nodiscard]] auto clone() const -> Any override;
  };

  namespace lambda {
    template<typename>
    struct transmute_argument {
      static constexpr auto into_type = meta::VariantType::ANY;
    };

    template<>
    struct transmute_argument<Intepreter&> {
      static constexpr auto into_type = meta::VariantType::UNIT;

      // ReSharper disable once CppMemberFunctionMayBeStatic
      auto transmute // NOLINT(*-convert-member-functions-to-static)
      (Intepreter &env, const Any &) -> Option<RefMut<Intepreter>> {
        return crab::some(RefMut{env});
      }
    };

    template<typename T>
    auto transmute_arg(Intepreter &, Any any) {
      return transmute_argument<T>{}(std::move(any));
    }

    inline auto transmute_arg(Intepreter &env, const Any &) -> Option<RefMut<Intepreter>> {
      return crab::some(RefMut{env});
    }

    template<typename T>
    struct transmute_arg_wrapper {
      auto operator()(Intepreter &, const Vec<Any> &any, usize &i) const {
        return transmute_argument<T>{}(any.at(i++));
      }
    };

    template<>
    struct transmute_arg_wrapper<Ref<Intepreter>> {
      auto operator()(const Intepreter &env, const Vec<Any> &, usize &) const -> Option<Ref<Intepreter>> {
        return crab::some(Ref{env});
      }
    };

    template<>
    struct transmute_arg_wrapper<RefMut<Intepreter>> {
      auto operator()(Intepreter &env, const Vec<Any> &, usize &) const -> Option<RefMut<Intepreter>> {
        return crab::some(RefMut{env});
      }
    };

    template<>
    struct transmute_argument<Any> {
      static constexpr auto into_type = meta::VariantType::ANY;

      auto operator()(Any any) const -> Option<Any> {
        return Option<Any>{std::move(any)};
      }
    };

    template<type::specialized_value T>
    struct transmute_argument<RcMut<T>> {
      static constexpr auto into_type = T::TYPE;

      auto operator()(Any any) const -> Option<RcMut<T>> {
        return any.downcast<T>();
      }
    };

    template<typename T>
    struct transmute_argument<Ref<RcMut<T>>> {
      static constexpr meta::VariantType into_type = transmute_argument<RcMut<T>>::into_type;

      auto operator()(Any any) const -> Option<RcMut<T>> {
        return transmute_argument<RcMut<T>>{}(any);
      }
    };

    template<typename T>
    struct transmute_argument<Ref<Rc<T>>> {
      static constexpr meta::VariantType into_type = transmute_argument<RcMut<T>>::into_type;

      auto operator()(Any any) const -> Option<Rc<T>> {
        return transmute_argument<Rc<T>>{}(any);
      }
    };

    template<typename T>
    struct transmute_argument<RefMut<RcMut<T>>> {
      static constexpr meta::VariantType into_type = transmute_argument<RcMut<T>>::into_type;

      auto operator()(Any any) const -> Option<RcMut<T>> {
        return transmute_argument<RcMut<T>>{}(any);
      }
    };

    template<typename T>
    struct transmute_argument<RefMut<Rc<T>>> {
      static constexpr meta::VariantType into_type = transmute_argument<RcMut<T>>::into_type;

      auto operator()(Any any) const -> Option<Rc<T>> {
        return transmute_argument<Rc<T>>{}(any);
      }
    };

    template<type::value_type T>
    struct transmute_argument<Ref<T>> {
      static constexpr meta::VariantType into_type = T::TYPE;

      auto operator()(Any any) const -> Option<Ref<T>> {
        Option<RcMut<T>> casted = any.downcast<T>();
        if (casted.is_none()) return crab::none;
        return crab::some(Ref{*casted.take_unchecked()});
      }
    };

    template<type::value_type T>
    struct transmute_argument<RefMut<T>> {
      static constexpr meta::VariantType into_type = T::TYPE;

      auto operator()(Any any) const -> Option<RefMut<T>> {
        Option<RcMut<T>> casted = any.downcast<T>();
        if (casted.is_none()) return crab::none;
        return crab::some(RefMut{*casted.take_unchecked()});
      }
    };

    template<>
    struct transmute_argument<Ref<Any>> {
      static constexpr meta::VariantType into_type = meta::VariantType::ANY;

      auto operator()(Any any) const -> Option<Any> {
        return Option{std::move(any)};
      }
    };

    template<type::convertible_primitive Primitive>
    struct transmute_argument<Primitive> {
      static constexpr meta::VariantType into_type = type::primitive_to_goose_t<Primitive>::TYPE;

      auto operator()(const Any &any) const -> Option<Primitive> {
        return type::try_from_goose<Primitive>(any);
      }
    };

    template<type::convertible_primitive Primitive>
    struct transmute_argument<Ref<Primitive>> {
      static constexpr meta::VariantType into_type = type::primitive_to_goose_t<Primitive>::TYPE;

      auto operator()(const Any &any) const -> Option<Primitive> {
        return type::try_from_goose<Primitive>(any);
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

    template<type::value_type T>
    struct transmute_return<RcMut<T>> {
      auto operator()(RcMut<T> returned) const -> Result<Any> {
        return Result<Any>{Any{returned}};
      }
    };

    template<type::value_type T>
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
    auto opt_to_err(Intepreter &runtime, const Vec<Any> &args, usize &index) -> Option<Box<err::Error>> {
      if constexpr (std::same_as<Into, Intepreter&>) {
        return crab::none;
      }

      constexpr transmute_arg_wrapper<typename crab::ref::decay_type<Into>::type> transmute{};

      using transmute_t = transmute_argument<typename crab::ref::decay_type<Into>::type>;

      if (index >= args.size()) {
        return err::make<err::InvalidCast>(meta::VariantType::NIL, transmute_t::into_type).value;
      }

      const usize p_index = index;

      if (transmute(runtime, args, index).is_some()) {
        return crab::none;
      };

      return err::make<err::InvalidCast>(args.at(p_index)->get_type(), transmute_t::into_type).value;
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
        [function](Intepreter &env, const Vec<Any> &args) -> Result<Any> {
          usize i = 0;

          // std::wcout << "::" << args.at(0)->to_string() << std::endl;
          if (args.at(0)->to_string() == L"42") {
            std::wcout << "";
          }

          if (auto typed_args = lambda::fold_option<Box<err::Error>>(
            lambda::opt_to_err<Parameters>(env, args, i)...
          ); typed_args.is_some()) {
            return crab::err(typed_args.take_unchecked());
          }

          i = 0;

          if constexpr (not std::is_same_v<Output, void>) {
            // TODO type checking
            return transmute_return<Output>{}(
              function(
                transmute_arg_wrapper<typename crab::ref::decay_type<Parameters>::type>{}(env, args, i).
                take_unchecked()
                ...
              )
            );
          }
          function(
            transmute_arg_wrapper<typename crab::ref::decay_type<Parameters>::type>{}(env, args, i).
            take_unchecked()
            ...
          );
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
