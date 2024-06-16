//
// Created by bishan_ on 6/15/24.
//

#pragma once
#include <preamble.hpp>
#include "Boolean.hpp"
#include "Decimal.hpp"
#include "GString.hpp"
#include "Integer.hpp"
#include "Nil.hpp"
#include "Unit.hpp"
#include "ast/Statement.hpp"

namespace goos::runtime::type {
  struct no_matching_primitive {};

  struct no_matching_wrapper {};

  template<typename T>
  concept is_value = std::derived_from<T, IValue> ;

  template<typename T>
  concept is_concrete_value = is_value<T> and not std::is_abstract_v<T>;

  template<typename T> requires std::derived_from<T, IValue>
  struct goos_to_primitive {
    using type = no_matching_primitive;
  };

  template<typename>
  struct primitive_to_goos {
    using type = no_matching_wrapper;
  };

  template<typename T>
  using goos_to_primitive_t = typename goos_to_primitive<T>::type;

  template<typename T>
  using primitive_to_goose_t = typename primitive_to_goos<T>::type;

  /**
   * @brief Is 'Primitive' a type that can be converted from C++ -> Goos
   */
  template<typename Primitive>
  concept is_convertible_primitive = not std::is_same_v<typename primitive_to_goose_t<Primitive>::type,
    no_matching_primitive>;

  // Specific Template Instantiations

  #define goos_define_from_cpp(cpp_primitive, wrapper)\
  template<> \
  struct primitive_to_goos<cpp_primitive> {\
    using type = wrapper;\
  };

  #define goos_define_to_cpp(wrapper, cpp_primitive)\
  template<> \
  struct goos_to_primitive<wrapper> {\
    using type = cpp_primitive;\
  };

  // Boolean
  goos_define_from_cpp(bool, Boolean);

  goos_define_to_cpp(Boolean, bool);

  // Integer
  goos_define_from_cpp(i64, Integer);

  goos_define_from_cpp(long long, Integer);

  goos_define_from_cpp(i32, Integer);

  goos_define_from_cpp(i16, Integer);

  goos_define_from_cpp(i8, Integer);

  goos_define_from_cpp(u64, Integer);

  goos_define_from_cpp(u32, Integer);

  goos_define_from_cpp(u16, Integer);

  goos_define_from_cpp(u8, Integer);

  goos_define_to_cpp(Integer, i64);

  // Decimal
  goos_define_from_cpp(f64, Decimal);

  goos_define_from_cpp(double, Decimal);

  goos_define_from_cpp(f32, Decimal);

  goos_define_to_cpp(Decimal, f64);

  // String Types
  goos_define_from_cpp(WideString, GString);

  goos_define_from_cpp(WideStringView, GString);

  goos_define_to_cpp(GString, const WideString&);

  // Monostate / 0 sized types
  goos_define_from_cpp(unit, Unit);

  goos_define_to_cpp(Unit, unit);

  goos_define_from_cpp(std::nullptr_t, Nil);

  #undef goos_define_from_cpp

  // Conversions
  template<typename Wrapper> requires is_concrete_value<Wrapper>
  auto from_goos(RcMut<Wrapper> wrapper) -> goos_to_primitive_t<Wrapper> {
    return wrapper->get();
  }

  template<typename T>
  auto to_goos(T primitive) -> RcMut<primitive_to_goose_t<T>> {
    return crab::make_rc_mut<primitive_to_goose_t<T>>(
      static_cast<goos_to_primitive_t<primitive_to_goose_t<T>>>(std::move(primitive))
    );
  }

  template<typename Primitive>
  auto try_from_goose(Any wrapper) -> Option<Primitive> {
    if (auto specialized = wrapper.downcast<primitive_to_goose_t<Primitive>>()) {
      return crab::some(static_cast<Primitive>(specialized.take_unchecked()->get()));
    }

    // Identity case.
    if constexpr (is_concrete_value<Primitive>) {
      return wrapper.downcast<Primitive>();
    }
    return crab::none;
  }

  template<>
  inline auto try_from_goose<Any>(Any wrapper) -> Option<Any> {
    return crab::some(std::move(wrapper));
  }

  template<typename T>
  auto to_goos_any(T primitive) -> Any {
    return to_goos(std::move(primitive));
  }

  // Identitiy function if trying to convert a value to a value
  template<typename T> requires std::is_base_of_v<T, IValue>
  auto to_goos(RcMut<T> primitive) -> RcMut<T> { return primitive; }
}
