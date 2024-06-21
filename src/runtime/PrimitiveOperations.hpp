//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include <cmath>
#include <functional>
#include <preamble.hpp>
#include <typeindex>
#include <typeinfo>
#include <lexer/Operator.hpp>

#include "ast/Statement.hpp"
#include "data/Boolean.hpp"
#include "data/Decimal.hpp"
#include "data/Integer.hpp"
#include "data/GString.hpp"
#include "data/TypeConversion.hpp"
#include "data/Array.hpp"

namespace goos::runtime::primitive_operators {
  struct BinaryOperands;
  struct UnaryOperands;
}

template<>
struct std::hash<goos::runtime::primitive_operators::BinaryOperands> {
  hash() = default;

  auto operator()(const goos::runtime::primitive_operators::BinaryOperands &ty) const noexcept -> usize;
};

template<>
struct std::hash<goos::runtime::primitive_operators::UnaryOperands> {
  hash() = default;

  auto operator()(const goos::runtime::primitive_operators::UnaryOperands &ty) const noexcept -> usize;
};

namespace goos::runtime::primitive_operators {
  struct BinaryOperands {
    meta::VariantType lhs, rhs;
    lexer::Operator op;

    auto operator==(const BinaryOperands &other) const -> bool;
  };

  struct UnaryOperands {
    meta::VariantType operand;
    lexer::Operator op;

    auto operator==(const UnaryOperands &other) const -> bool;
  };

  using BinaryFunction = std::function<Any(Any lhs, Any rhs)>;
  using UnaryFunction = std::function<Any(Any val)>;

  namespace meta {
    template<
      lexer::Operator op,
      type::convertible_primitive Lhs,
      type::convertible_primitive Rhs,
      type::convertible_primitive Output
    >
    auto binary_func(
      std::function<Output(Lhs, Rhs)> function
    ) -> std::pair<BinaryOperands, BinaryFunction> {
      return {
        BinaryOperands{type::variant_type_of_primitive<Lhs>, type::variant_type_of_primitive<Rhs>, op},
        [function](const Any &lhs, const Any &rhs) -> Any {
          return type::to_goos_any(
            function(
              type::try_from_goose<Lhs>(lhs).take_unchecked(),
              type::try_from_goose<Rhs>(rhs).take_unchecked()
            )
          );
        }
      };
    }

    template<
      lexer::Operator op,
      typename F
    >
    auto binary(const F function) -> std::pair<BinaryOperands, BinaryFunction> {
      return binary_func<op>(std::function{function});
    }

    template<
      lexer::Operator op,
      type::convertible_primitive Input,
      type::convertible_primitive Output
    >
    auto unary_func(
      std::function<Output(Input)> function
    ) -> std::pair<UnaryOperands, UnaryFunction> {
      static_assert(is_unary(op));

      return {
        UnaryOperands{type::variant_type_of_primitive<Input>, op},
        [function](const Any &input) -> Any {
          return type::to_goos_any(
            function(
              type::try_from_goose<Input>(input).take_unchecked()
            )
          );
        }
      };
    }

    template<
      lexer::Operator op,
      typename F
    >
    auto unary(const F function) -> std::pair<UnaryOperands, UnaryFunction> {
      return meta::unary_func<op>(std::function{function});
    }
  }

  inline static const Dictionary<BinaryOperands, BinaryFunction> BINARY_FUNCTIONS = [] {
    using namespace meta;
    using namespace goos::meta;
    using lexer::Operator;

    #define operation(op, Lhs, Rhs, lambda) binary<Operator::op>([](const Lhs lhs, const Rhs rhs) { return lambda; })

    Dictionary<BinaryOperands, BinaryFunction> map{
      // Bolean Operations
      // operation(AND, Boolean, Boolean, Boolean, lhs and rhs),
      operation(AND, bool, bool, lhs == rhs),
      operation(NOT_EQUALS, bool, bool, lhs != rhs),
      operation(OR, bool, bool, lhs or rhs),

      // String Concatanation
      operation(ADD, WideString&, WideString&, lhs + rhs),
      operation(ADD, WideString&, i64, lhs + std::to_wstring(rhs)),
      operation(ADD, WideString&, f64, lhs + std::to_wstring(rhs)),
      operation(EQUALS, WideString&, WideString&, lhs == rhs),

      // TODO remove this for better method solution, this shit cringe af
      // #define ARRAY_APPEND(ty) std::make_pair<BinaryOperands, BinaryFunction>(\
      //   BinaryOperands{VariantType::ARRAY, VariantType::ty, Operator::ADD_ASSIGN},\
      //   [](Any lhs, Any rhs) {\
      //     RcMut<Array> lhs_arr = crab::unwrap(lhs.downcast<Array>());\
      //     lhs_arr->push(std::move(rhs));\
      //     return lhs;\
      //   }\
      // )
      //
      // ARRAY_APPEND(BOOLEAN),
      // ARRAY_APPEND(ARRAY),
      // ARRAY_APPEND(INTEGER),
      // ARRAY_APPEND(DECIMAL),
      // ARRAY_APPEND(STRING),
      // ARRAY_APPEND(BOOLEAN),
      // ARRAY_APPEND(OBJECT),
      // ARRAY_APPEND(ARRAY),
      // ARRAY_APPEND(FUNCTION),
      // ARRAY_APPEND(NIL),
      // ARRAY_APPEND(UNIT),
      // ARRAY_APPEND(REFERENCE),
      // ARRAY_APPEND(ANY),
      #undef ARRAY_APPEND

      // Base i64 Operations
      #define numeric_op(op_enum, op) \
        operation(op_enum, f64, f64, lhs op rhs ),\
        operation(op_enum, i64, i64, lhs op rhs ),\
        operation(op_enum, i64, f64, static_cast<f64>(lhs) op rhs ),\
        operation(op_enum, f64, i64, lhs op static_cast<f64>(rhs))

      // Bit Operations
      operation(XOR, i64, i64, lhs xor rhs),
      operation(BIT_OR, i64, i64, lhs bitor rhs),
      operation(BIT_AND, i64, i64, lhs bitand rhs),
      operation(SHIFT_LEFT, i64, i64, lhs << rhs),
      operation(SHIFT_RIGHT, i64, i64, lhs >> rhs),

      operation(EQUALS, i64, i64, lhs == rhs),
      operation(NOT_EQUALS, i64, i64, lhs != rhs),
      numeric_op(GREATER, >),
      numeric_op(LESS, <),
      numeric_op(GREATER_OR_EQUALS, >=),
      numeric_op(LESS_OR_EQUALS, <=),

      operation(MOD, i64, i64, lhs % rhs),

      numeric_op(ADD, +),
      numeric_op(SUB, -),
      numeric_op(MUL, *),
      numeric_op(DIV, /),

      numeric_op(ADD, +),

      // Floating Point
      operation(ADD, f64, f64, lhs + rhs),
      operation(MOD, f64, f64, std::fmod(lhs, rhs)),

      operation(EQUALS, f64, f64, std::abs(lhs - rhs) < 1E-6_f64),
      operation(EQUALS, i64, f64, std::abs(static_cast<f64>(lhs) - rhs) < 1E-6_f64),
      operation(EQUALS, f64, i64, std::abs(static_cast<f64>(rhs) - lhs) < 1E-6_f64),
      operation(NOT_EQUALS, f64, f64, std::abs(lhs - rhs) > 1E-6_f64),
      operation(NOT_EQUALS, i64, f64, std::abs(static_cast<f64>(lhs) - rhs) > 1E-6_f64),
      operation(NOT_EQUALS, f64, i64, std::abs(static_cast<f64>(rhs) - lhs) > 1E-6_f64),
    };
    #undef numeric_op
    #undef operation

    return map;
  }();

  inline static const Dictionary<UnaryOperands, UnaryFunction> UNARY_FUNCTIONS = [] {
    using namespace meta;
    using lexer::Operator;

    #define operation(op, Input, lambda) unary<Operator::op>([](const Input val) {  return lambda; } )

    Dictionary<UnaryOperands, UnaryFunction> map{
      operation(NOT, bool, not val),
      operation(SUB, i64, -val),
      operation(SUB, f64, -val),
    };

    #undef operation
    return map;
  }();
}
