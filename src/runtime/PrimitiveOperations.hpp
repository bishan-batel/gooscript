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

namespace goos::runtime::primitive_operators {
  struct Operands;
}

template<>
struct std::hash<goos::runtime::primitive_operators::Operands> {
  hash() = default;

  auto operator()(const goos::runtime::primitive_operators::Operands &ty) const noexcept -> usize;
};

namespace goos::runtime::primitive_operators {
  struct Operands {
    meta::VariantType lhs, rhs;
    lexer::Operator op;

    auto operator==(const Operands &other) const -> bool;
  };

  using BinaryFunction = std::function<Any(Any lhs, Any rhs)>;

  namespace meta {
    template<lexer::Operator op, typename Lhs, typename Rhs=Lhs, typename WrapperOutput=Lhs, typename F>
    auto binary(
      F function
    ) -> std::pair<Operands, BinaryFunction> {
      return {
        Operands{Lhs{}.get_type(), Rhs{}.get_type(), op},
        [function](Any lhs_any, Any rhs_any) {
          RcMut<Lhs> lhs = crab::unwrap(lhs_any.downcast<Lhs>());
          RcMut<Rhs> rhs = crab::unwrap(rhs_any.downcast<Rhs>());
          return crab::make_rc_mut<WrapperOutput>(function(lhs->get(), rhs->get()));
        }
      };
    }
  }

  inline static const Dictionary<Operands, BinaryFunction> BINARY_FUNCTIONS = [] {
    using namespace meta;
    using lexer::Operator;

    Dictionary<Operands, BinaryFunction> map{
      binary<Operator::AND, Boolean>(
        [](const auto &lhs, const auto &rhs) { return lhs and rhs; }
      ),

      binary<Operator::NOT_EQUALS, Boolean>(
        [](const auto &lhs, const auto &rhs) { return lhs != rhs; }
      ),

      binary<Operator::OR, Boolean>(
        [](const auto &lhs, const auto &rhs) { return lhs or rhs; }
      ),

      binary<Operator::ADD, GString>(
        [](const auto &lhs, const auto &rhs) { return lhs + rhs; }
      ),

      binary<Operator::ADD, GString, Integer>(
        [](const auto &lhs, const auto &rhs) { return lhs + std::to_wstring(rhs); }
      ),

      binary<Operator::ADD, GString, Decimal>(
        [](const auto &lhs, const auto &rhs) { return lhs + std::to_wstring(rhs); }
      ),

      binary<Operator::ADD, Integer>(
        [](const auto lhs, const auto rhs) { return lhs + rhs; }
      ),

      binary<Operator::XOR, Integer>(
        [](const auto lhs, const auto rhs) { return lhs xor rhs; }
      ),

      binary<Operator::BIT_OR, Integer>(
        [](const auto lhs, const auto rhs) { return lhs bitor rhs; }
      ),

      binary<Operator::BIT_AND, Integer>(
        [](const auto lhs, const auto rhs) { return lhs bitand rhs; }
      ),

      binary<Operator::EQUALS, Integer, Integer, Boolean>(
        [](const auto lhs, const auto rhs) { return lhs == rhs; }
      ),

      binary<Operator::NOT_EQUALS, Integer, Integer, Boolean>(
        [](const auto lhs, const auto rhs) { return lhs != rhs; }
      ),

      binary<Operator::GREATER, Integer, Integer, Boolean>(
        [](const auto lhs, const auto rhs) { return lhs > rhs; }
      ),

      binary<Operator::LESS, Integer, Integer, Boolean>(
        [](const auto lhs, const auto rhs) { return lhs < rhs; }
      ),

      binary<Operator::GREATER_OR_EQUALS, Integer, Integer, Boolean>(
        [](const auto lhs, const auto rhs) { return lhs >= rhs; }
      ),

      binary<Operator::LESS_OR_EQUALS, Integer, Integer, Boolean>(
        [](const auto lhs, const auto rhs) { return lhs <= rhs; }
      ),

      binary<Operator::SHIFT_LEFT, Integer>(
        [](const auto lhs, const auto rhs) { return lhs << rhs; }
      ),

      binary<Operator::SHIFT_RIGHT, Integer>(
        [](const auto lhs, const auto rhs) { return lhs >> rhs; }
      ),

      binary<Operator::ADD, Integer>(
        [](const auto lhs, const auto rhs) { return lhs + rhs; }
      ),

      binary<Operator::SUB, Integer>(
        [](const auto lhs, const auto rhs) { return lhs - rhs; }
      ),

      binary<Operator::MUL, Integer>(
        [](const auto lhs, const auto rhs) { return lhs * rhs; }
      ),

      binary<Operator::DIV, Integer>(
        [](const auto lhs, const auto rhs) { return lhs / rhs; }
      ),

      binary<Operator::MOD, Integer>(
        [](const auto lhs, const auto rhs) { return lhs % rhs; }
      ),

      // Floating Point
      binary<Operator::ADD, Decimal>(
        [](const auto lhs, const auto rhs) { return lhs + rhs; }
      ),

      binary<Operator::EQUALS, Decimal, Decimal, Boolean>(
        [](const auto lhs, const auto rhs) { return lhs == rhs; }
      ),

      binary<Operator::NOT_EQUALS, Decimal, Decimal, Boolean>(
        [](const auto lhs, const auto rhs) { return lhs != rhs; }
      ),

      binary<Operator::GREATER, Decimal, Decimal, Boolean>(
        [](const auto lhs, const auto rhs) { return lhs > rhs; }
      ),

      binary<Operator::LESS, Decimal, Decimal, Boolean>(
        [](const auto lhs, const auto rhs) { return lhs < rhs; }
      ),

      binary<Operator::GREATER_OR_EQUALS, Decimal, Decimal, Boolean>(
        [](const auto lhs, const auto rhs) { return lhs >= rhs; }
      ),

      binary<Operator::LESS_OR_EQUALS, Decimal, Decimal, Boolean>(
        [](const auto lhs, const auto rhs) { return lhs <= rhs; }
      ),

      binary<Operator::ADD, Decimal>(
        [](const auto lhs, const auto rhs) { return lhs + rhs; }
      ),

      binary<Operator::SUB, Decimal>(
        [](const auto lhs, const auto rhs) { return lhs - rhs; }
      ),

      binary<Operator::MUL, Decimal>(
        [](const auto lhs, const auto rhs) { return lhs * rhs; }
      ),

      binary<Operator::DIV, Decimal>(
        [](const auto lhs, const auto rhs) { return lhs / rhs; }
      ),

      binary<Operator::MOD, Decimal>(
        [](const auto lhs, const auto rhs) { return std::fmod(lhs, rhs); }
      )

    };
    return map;
  }();

  class PrimitiveOperations {};
} // goos
