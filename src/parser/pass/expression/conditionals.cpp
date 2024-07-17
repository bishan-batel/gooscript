//
// Created by bishan_ on 5/8/24.
//
#include "conditionals.hpp"
#include "ast/expression/Unary.hpp"
#include "expression.hpp"

namespace goos::parser::pass::expr {
  auto if_condition(TokenStream &stream) -> OptionalResult<ast::expression::If> {
    ast::TokenTrace full_trace = stream.trace();

    const bool invert = stream.is_curr(lexer::Keyword::UNLESS);

    if (not(stream.try_consume(lexer::Keyword::IF) or stream.try_consume(lexer::Keyword::UNLESS))) {
      return OptionalResult<ast::expression::If>{crab::none};
    }

    auto result_cond{expression(stream)};
    if (result_cond.is_err())
      return crab::err(result_cond.take_err_unchecked());

    auto condition = result_cond.take_unchecked();

    auto condition_trace = condition->token_trace();

    if (invert)
      condition = crab::make_box<ast::expression::Unary>(lexer::Operator::NOT, std::move(condition), condition_trace);

    Box<ast::Expression> then{crab::make_box<ast::expression::Unit>(full_trace)};

    if (stream.try_consume(lexer::Operator::CURLY_OPEN)) {
      auto result{statements_list(stream, [](TokenStream &s) { return s.try_consume(lexer::Operator::CURLY_CLOSE); })};
      if (result.is_err())
        return crab::err(result.take_err_unchecked());
      then = result.take_unchecked();
    } else if (stream.try_consume(lexer::Keyword::THEN)) {
      auto result{expression(stream)};

      if (result.is_err())
        return crab::err(result.take_err_unchecked());

      then = result.take_unchecked();
    } else {
      return crab::err(stream.unexpected("'then' or '{' "));
    }

    Box<ast::Expression> else_then{crab::make_box<ast::expression::Unit>(full_trace)};

    if (stream.try_consume(lexer::Keyword::ELSE)) {
      if (stream.try_consume(lexer::Operator::CURLY_OPEN)) {
        auto result{statements_list(stream, [](auto &s) { return s.try_consume(lexer::Operator::CURLY_CLOSE); })};
        if (result.is_err()) {
          return crab::err(result.take_err_unchecked());
        }
        else_then = result.take_unchecked();
      } else {
        // "else bruh()" or "else if ..."
        auto result{expression(stream)};

        if (result.is_err()) {
          return crab::err(result.take_err_unchecked());
        }
        else_then = result.take_unchecked();
      }
    }

    full_trace = full_trace.merge(stream.trace());

    return crab::ok(crab::some(
        crab::make_box<ast::expression::If>(std::move(condition), std::move(then), std::move(else_then), full_trace)));
  }

  auto while_loop([[maybe_unused]] TokenStream &stream) -> OptionalResult<ast::expression::While> {
    const bool invert = stream.is_curr(lexer::Keyword::UNTIL);

    ast::TokenTrace full_trace = stream.trace();

    if (not(stream.try_consume(lexer::Keyword::WHILE) or stream.try_consume(lexer::Keyword::UNTIL))) {
      return OptionalResult<ast::expression::While>{crab::none};
    }

    auto result_cond{expression(stream)};
    if (result_cond.is_err())
      return crab::err(result_cond.take_err_unchecked());

    auto condition = result_cond.take_unchecked();
    const auto condition_trace = condition->token_trace();

    if (invert)
      condition = crab::make_box<ast::expression::Unary>(lexer::Operator::NOT, std::move(condition), condition_trace);

    Box<ast::Expression> then_do{crab::make_box<ast::expression::Unit>(full_trace)};

    if (stream.try_consume(lexer::Operator::CURLY_OPEN)) {
      auto result{statements_list(stream, [](TokenStream &s) { return s.try_consume(lexer::Operator::CURLY_CLOSE); })};
      if (result.is_err())
        return crab::err(result.take_err_unchecked());
      then_do = result.take_unchecked();
    } else if (stream.try_consume(lexer::Keyword::DO)) {
      auto result{expression(stream)};

      if (result.is_err())
        return crab::err(result.take_err_unchecked());

      then_do = result.take_unchecked();
    } else {
      return crab::err(stream.unexpected("'do' or '{' "));
    }

    full_trace = full_trace.merge(stream.trace());
    return crab::ok(
        crab::some(crab::make_box<ast::expression::While>(std::move(condition), std::move(then_do), full_trace)));
  }

  auto for_loop([[maybe_unused]] TokenStream &stream) -> OptionalResult<ast::expression::If> {
    // TODO
    return OptionalResult<ast::expression::If>{crab::none};
  }
} // namespace goos::parser::pass::expr
