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
#include "token/Array.hpp"

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
    template<lexer::Operator op, typename Lhs, typename Rhs=Lhs, typename WrapperOutput=Lhs, typename F>
    auto binary(
      F function
    ) -> std::pair<BinaryOperands, BinaryFunction> {
      static_assert(is_binary(op));
      return {
        BinaryOperands{Lhs{}.get_type(), Rhs{}.get_type(), op},
        [function](Any lhs_any, Any rhs_any) {
          RcMut<Lhs> lhs = crab::unwrap(lhs_any.downcast<Lhs>());
          RcMut<Rhs> rhs = crab::unwrap(rhs_any.downcast<Rhs>());
          return crab::make_rc_mut<WrapperOutput>(function(lhs->get(), rhs->get()));
        }
      };
    }

    template<lexer::Operator op, type::specialized_value Input, type::specialized_value WrapperOutput, typename F>
    auto unary(
      F function
    ) -> std::pair<UnaryOperands, UnaryFunction> {
      static_assert(is_unary(op));
      return {
        UnaryOperands{Input{}.get_type(), op},
        [function](Any value) {
          return crab::make_rc_mut<WrapperOutput>(
            function(
              crab::unwrap(value.downcast<Input>())->get()
            )
          );
        }
      };
    }
  }

  inline static const Dictionary<BinaryOperands, BinaryFunction> BINARY_FUNCTIONS = [] {
    using namespace meta;
    using namespace goos::meta;
    using lexer::Operator;

    #define operation(op, Lhs, Rhs, Output, lambda) binary<Operator::op, Lhs, Rhs, Output>([](const auto&lhs, const auto&rhs) {  return lambda; } )

    Dictionary<BinaryOperands, BinaryFunction> map{
      // Bolean Operations
      operation(AND, Boolean, Boolean, Boolean, lhs and rhs),
      operation(NOT_EQUALS, Boolean, Boolean, Boolean, lhs != rhs),
      operation(OR, Boolean, Boolean, Boolean, lhs or rhs),

      // String Concatanation
      operation(ADD, GString, GString, GString, *lhs + *rhs),
      operation(ADD, GString, Integer, GString, *lhs + std::to_wstring(rhs)),
      operation(ADD, GString, Decimal, GString, *lhs + std::to_wstring(rhs)),
      operation(EQUALS, GString, GString, Boolean, *lhs == *rhs),

      // TODO remove this for better method solution, this shit cringe af
      #define ARRAY_APPEND(ty) std::make_pair<BinaryOperands, BinaryFunction>(\
        BinaryOperands{VariantType::ARRAY, VariantType::ty, Operator::ADD_ASSIGN},\
        [](Any lhs, Any rhs) {\
          RcMut<Array> lhs_arr = crab::unwrap(lhs.downcast<Array>());\s_arr->push(std::move(rhs));\
          return lhs;\
        }\
      )

      ARRAY_APPEND(BOOLEAN),
      ARRAY_APPEND(ARRAY),
      ARRAY_APPEND(INTEGER),
      ARRAY_APPEND(DECIMAL),
      ARRAY_APPEND(STRING),
      ARRAY_APPEND(BOOLEAN),
      ARRAY_APPEND(OBJECT),
      ARRAY_APPEND(ARRAY),
      ARRAY_APPEND(FUNCTION),
      ARRAY_APPEND(NIL),
      ARRAY_APPEND(UNIT),
      ARRAY_APPEND(REFERENCE),
      ARRAY_APPEND(ANY),
      #undef ARRAY_APPEND

      // Base Integer Operations
      operation(ADD, Integer, Integer, Integer, lhs + rhs),
      operation(XOR, Integer, Integer, Integer, lhs xor rhs),
      operation(BIT_OR, Integer, Integer, Integer, lhs bitor rhs),
      operation(BIT_AND, Integer, Integer, Integer, lhs bitand rhs),
      operation(EQUALS, Integer, Integer, Boolean, lhs == rhs),
      operation(NOT_EQUALS, Integer, Integer, Boolean, lhs != rhs),
      operation(GREATER, Integer, Integer, Boolean, lhs > rhs),
      operation(LESS, Integer, Integer, Boolean, lhs < rhs),
      operation(GREATER_OR_EQUALS, Integer, Integer, Boolean, lhs >= rhs),
      operation(LESS_OR_EQUALS, Integer, Integer, Boolean, lhs <= rhs),
      operation(SHIFT_LEFT, Integer, Integer, Integer, lhs << rhs),
      operation(SHIFT_RIGHT, Integer, Integer, Integer, lhs >> rhs),
      operation(ADD, Integer, Integer, Integer, lhs + rhs),
      operation(SUB, Integer, Integer, Integer, lhs - rhs),
      operation(MUL, Integer, Integer, Integer, lhs * rhs),
      operation(DIV, Integer, Integer, Integer, lhs / rhs),
      operation(MOD, Integer, Integer, Integer, lhs % rhs),

      // Floating Point
      operation(ADD, Decimal, Decimal, Decimal, lhs + rhs),
      operation(EQUALS, Decimal, Decimal, Boolean, lhs == rhs),
      operation(NOT_EQUALS, Decimal, Decimal, Boolean, lhs != rhs),
      operation(GREATER, Decimal, Decimal, Boolean, lhs > rhs),
      operation(LESS, Decimal, Decimal, Boolean, lhs < rhs),
      operation(GREATER_OR_EQUALS, Decimal, Decimal, Boolean, lhs >= rhs),
      operation(LESS_OR_EQUALS, Decimal, Decimal, Boolean, lhs <= rhs),
      operation(ADD, Decimal, Decimal, Decimal, lhs + rhs),
      operation(SUB, Decimal, Decimal, Decimal, lhs - rhs),
      operation(MUL, Decimal, Decimal, Decimal, lhs * rhs),
      operation(DIV, Decimal, Decimal, Decimal, lhs / rhs),
      operation(MOD, Decimal, Decimal, Decimal, std::fmod(lhs, rhs)),
    };
    #undef operation

    return map;
  }();

  inline static const Dictionary<UnaryOperands, UnaryFunction> UNARY_FUNCTIONS = [] {
    using namespace meta;
    using lexer::Operator;

    #define operation(op, Input, Output, lambda) unary<Operator::op, Input, Output>([](const auto& val) {  return lambda; } )

    Dictionary<UnaryOperands, UnaryFunction> map{
      operation(NOT, Boolean, Boolean, not val),
      operation(SUB, Integer, Integer, -val),
      operation(SUB, Decimal, Decimal, -val),
    };

    #undef operation
    return map;
  }();
}
