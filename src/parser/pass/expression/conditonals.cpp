//
// Created by bishan_ on 5/8/24.
//
#include "conditionals.hpp"
#include "expression.hpp"

namespace goos::parser::pass::expr {
  auto if_condition(TokenStream &stream) -> OptionalResult<ast::expression::If> {
    if (not stream.try_consume(lexer::Keyword::IF)) {
      return OptionalResult<ast::expression::If>{crab::none};
    }

    auto result_cond{expression(stream)};
    if (result_cond.is_err())
      return crab::err(result_cond.take_err_unchecked());

    auto condition = result_cond.take_unchecked();

    Box<ast::Expression> then{crab::make_box<ast::expression::Unit>()};

    if (stream.try_consume(lexer::Operator::CURLY_OPEN)) {
      auto result{statements_list(stream, [](auto &s) { return s.try_consume(lexer::Operator::CURLY_CLOSE); })};
      if (result.is_err()) return crab::err(result.take_err_unchecked());
      then = result.take_unchecked();
    } else if (stream.try_consume(lexer::Keyword::THEN)) {
      auto result{expression(stream)};

      if (result.is_err()) return crab::err(result.take_err_unchecked());

      then = result.take_unchecked();
    }

    Box<ast::Expression> else_then{crab::make_box<ast::expression::Unit>()};

    if (stream.try_consume(lexer::Keyword::ELSE)) {
      // else {...}
      if (stream.try_consume(lexer::Operator::CURLY_OPEN)) {
        auto result{statements_list(stream, [](auto &s) { return s.try_consume(lexer::Operator::CURLY_CLOSE); })};
        if (result.is_err()) return crab::err(result.take_err_unchecked());
        else_then = result.take_unchecked();
      } else {
        // "else bruh()" or "else if ..."
        auto result{expression(stream)};

        if (result.is_err()) return crab::err(result.take_err_unchecked());

        else_then = result.take_unchecked();
      }
    }

    return crab::ok(
      crab::some(
        crab::make_box<ast::expression::If>(std::move(condition), std::move(then), std::move(else_then))
      )
    );
  }

  auto while_loop([[maybe_unused]] TokenStream &stream) -> OptionalResult<ast::expression::If> {
    return OptionalResult<ast::expression::If>{crab::none};
  }

  auto for_loop([[maybe_unused]] TokenStream &stream) -> OptionalResult<ast::expression::If> {
    return OptionalResult<ast::expression::If>{crab::none};
  }
}
