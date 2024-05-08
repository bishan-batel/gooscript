//
// Created by bishan_ on 5/8/24.
//
#include "literals.hpp"

#include "token/Decimal.hpp"
#include "token/Integer.hpp"
#include "token/StringLiteral.hpp"

namespace goos::parser::pass::expr {
  auto decimal(TokenStream &stream) -> OptionalResult<ast::expression::Decimal> {
    if (auto decimal = stream.try_consume<token::Decimal>()) {
      const f64 number{decimal.take_unchecked()->get_number()};
      return crab::ok(crab::some(crab::make_box<ast::expression::Decimal>(number)));
    }

    return crab::ok<Option<Box<ast::expression::Decimal>>>(crab::none);
  }

  auto integer(TokenStream &stream) -> OptionalResult<ast::expression::Integer> {
    if (auto decimal = stream.try_consume<token::Integer>()) {
      const i64 number{decimal.take_unchecked()->get_number()};
      return crab::ok(crab::some(crab::make_box<ast::expression::Integer>(number)));
    }

    return crab::ok<Option<Box<ast::expression::Integer>>>(crab::none);
  }

  auto string(TokenStream &stream) -> OptionalResult<ast::expression::StringLiteral> {
    if (auto str = stream.try_consume<token::StringLiteral>()) {
      const WideString literal{str.take_unchecked()->get_string()};
      return crab::ok(
        crab::some(
          crab::make_box<ast::expression::StringLiteral>(std::move(literal))
        )
      );
    }

    return crab::ok<Option<Box<ast::expression::StringLiteral>>>(crab::none);
  }

  auto unit(TokenStream &stream) -> OptionalResult<ast::expression::Unit> {
    if (stream.try_consume(lexer::Keyword::UNIT)) {
      return crab::ok(crab::some(crab::make_box<ast::expression::Unit>()));
    }

    return crab::ok<Option<Box<ast::expression::Unit>>>(crab::none);
  }

  auto null(TokenStream &stream) -> OptionalResult<ast::expression::Nil> {
    if (stream.try_consume(lexer::Keyword::NIL)) {
      return crab::ok(crab::some(crab::make_box<ast::expression::Nil>()));
    }
    return crab::ok<Option<Box<ast::expression::Nil>>>(crab::none);
  }
}
