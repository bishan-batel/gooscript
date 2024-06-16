//
// Created by bishan_ on 5/7/24.
//

#pragma once
#include "conditionals.hpp"
#include "functions.hpp"
#include "literals.hpp"
#include "../statement/block.hpp"
#include "ast/Expression.hpp"
#include "parser/TokenStream.hpp"

namespace goos::parser::pass::expr {
  /**
   * Order of Operations Table
   */
  inline static const Vec<Set<lexer::Operator>> ORDER_OF_OPERATIONS{
    [] {
      using namespace lexer;

      return decltype(ORDER_OF_OPERATIONS){
        {},
        {Operator::COLON, Operator::DOT},
        {Operator::MUL, Operator::DIV, Operator::MOD},
        {Operator::ADD, Operator::SUB},
        // Vec{Operator::BitShiftLeft, Operator::BitShiftRight},
        {Operator::LESS, Operator::GREATER, Operator::LESS_OR_EQUALS, Operator::GREATER_OR_EQUALS},
        {Operator::EQUALS, Operator::NOT_EQUALS},
        {Operator::BIT_AND},
        {Operator::XOR},
        {Operator::BIT_OR},
        {Operator::AND},
        {Operator::OR},
        {
          Operator::MOD_ASSIGN,
          // Operator::XOR_ASSIGN,
          Operator::BIT_AND_ASSIGN,
          Operator::MUL_ASSIGN,
          Operator::SUB_ASSIGN,
          Operator::ADD_ASSIGN,
          Operator::DIV_ASSIGN,
          // Operator::BitShiftLeftAssign,
          // Operator::BitShiftRightAssign,
          // Operator::AndAssign,
          // Operator::OrAssign,
          Operator::ASSIGN
        }
      };
    }()
  };

  auto factor(TokenStream &stream) -> MustEvalResult<ast::Expression>;

  auto consume_binary_expression(TokenStream &stream, usize op_index) -> MustEvalResult<ast::Expression>;

  /**
   * @brief Evaluates to an expression or an error.
   */
  auto expression(TokenStream &stream) -> MustEvalResult<ast::Expression>;

  namespace meta {
    /**
     * @brief Used to group expression passes together
     */
    template<typename F> requires std::is_invocable_v<F, TokenStream&>
    auto transmute(F f) -> OptionalPass<ast::Expression> {
      return [f](TokenStream &stream) -> OptionalResult<ast::Expression> {
        auto result{f(stream)};
        if (result.is_err())
          return crab::err(result.take_err_unchecked());

        auto opt = result.take_unchecked();

        if (opt.is_none())
          return crab::ok<Option<Box<ast::Expression>>>(crab::none);

        return crab::ok<Option<Box<ast::Expression>>>(crab::some(Box<ast::Expression>{opt.take_unchecked()}));
      };
    }
  };

  // Optional Factor Passes

  inline static const std::vector FACTOR_PASSES{
    meta::transmute([](auto &s) { return block(s); }),
    meta::transmute(grouping),
    meta::transmute(if_condition),
    meta::transmute(while_loop),
    meta::transmute(for_loop),
    meta::transmute(array),
    meta::transmute(dictionary),
    meta::transmute(decimal),
    meta::transmute(integer),
    meta::transmute(string),
    meta::transmute(boolean),
    meta::transmute(unit),
    meta::transmute(null),
    meta::transmute(identifier_binding),
    meta::transmute(lambda),
  };
}
