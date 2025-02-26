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
#include "runtime/err/InvalidCast.hpp"

namespace goos::runtime::type {
  /**
   * @brief Any type that can be used as some form of IValue
   */
  template<typename T>
  concept value_type = std::derived_from<T, IValue> and requires {
    { T::TYPE } -> std::convertible_to<meta::VariantType>;
  };

  /**
   * @brief Any non-abstract value type.
   */
  template<typename T>
  concept specialized_value = value_type<T> and not std::is_abstract_v<T>;

  template<typename T> requires value_type<T>
  struct goos_to_primitive {};

  template<typename>
  struct primitive_to_goos {};

  /**
   * @brief If exists, this defines the type conversion from a standard goose type into a C++ 'native' type
   */
  template<typename T>
  using goos_to_primitive_t = typename goos_to_primitive<T>::type;

  /**
   * @brief If exists, this defines the standard goose type associated with the given datatype.
   */
  template<typename T>
  using primitive_to_goose_t = typename primitive_to_goos<T>::type;

  /**
   * @brief Is 'Primitive' a type that can be converted from C++ -> Goos
   */
  template<typename Primitive>
  concept convertible_primitive = requires() {
    typeid(primitive_to_goose_t<Primitive>);
  };

  template<typename>
  struct type_to_goose {};

  template<typename T>
  using type_to_goose_t = typename type_to_goose<T>::type;

  template<convertible_primitive T>
  struct type_to_goose<T> {
    using type = primitive_to_goose_t<T>;
  };

  template<value_type T>
  struct type_to_goose<T> {
    using type = T;
  };

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

  // Identities
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

  goos_define_from_cpp(const WideString&, GString);

  goos_define_from_cpp(Rc<WideString>, GString);

  goos_define_from_cpp(WideStringView, GString);

  goos_define_from_cpp(const widechar*, GString);

  goos_define_to_cpp(GString, Rc<WideString>);

  // Monostate / 0 sized types
  goos_define_from_cpp(unit, Unit);

  goos_define_to_cpp(Unit, unit);

  goos_define_from_cpp(std::nullptr_t, Nil);

  #undef goos_define_from_cpp

  template<specialized_value T>
  constexpr meta::VariantType variant_type_of = T::TYPE;

  template<convertible_primitive T>
  constexpr meta::VariantType variant_type_of_primitive = variant_type_of<primitive_to_goose_t<T>>;

  // Conversions
  template<specialized_value Wrapper>
  auto from_goos(RcMut<Wrapper> wrapper) -> goos_to_primitive_t<Wrapper> {
    return wrapper->get();
  }

  template<typename T>
  auto to_goos(T primitive) -> RcMut<primitive_to_goose_t<T>> {
    // return crab::make_rc_mut<primitive_to_goose_t<T>>(
    //   static_cast<goos_to_primitive_t<primitive_to_goose_t<T>>>(std::move(primitive))
    // );
    // TODO double check this to make sure the top part can be omitted
    return crab::make_rc_mut<primitive_to_goose_t<T>>(std::move(primitive));
  }

  template<convertible_primitive Primitive>
  auto try_from_goose(Any wrapper) -> Option<typename crab::ref::decay_type<Primitive>::type> {
    if (auto specialized = wrapper.downcast<primitive_to_goose_t<Primitive>>()) {
      return static_cast<typename crab::ref::decay_type<Primitive>::type>(specialized.take_unchecked()->get());
    }

    // Identity case.
    if constexpr (specialized_value<Primitive>) {
      return wrapper.downcast<Primitive>();
    }
    return crab::none;
  }

  template<value_type T>
  auto try_from_goose(Ref<RcMut<T>> wrapper) -> Option<primitive_to_goose_t<T>> {
    return type::try_from_goose(*wrapper);
  }

  inline auto try_from_goose(Any wrapper) -> Option<Any> {
    return crab::some(std::move(wrapper));
  }

  template<typename T>
  auto to_goos_any(T primitive) -> Any {
    return type::to_goos(std::move(primitive));
  }

  template<value_type T>
  auto to_goos_any(RcMut<T> primitive) -> Any {
    return primitive;
  }

  template<value_type Into, value_type From>
  auto coerce(RcMut<From> from) -> Result<RcMut<Into>, Box<err::Error>> {
    if (Option<RcMut<Into>> obj = from.template downcast<Into>(); obj.is_some())
      return obj.take_unchecked();

    return err::make<err::InvalidCast>(from->get_type(), variant_type_of<Into>);
  }

  // Identitiy function if trying to convert a value to a value
  template<typename T> requires std::is_base_of_v<T, IValue>
  auto to_goos(RcMut<T> primitive) -> RcMut<T> { return primitive; }
}
