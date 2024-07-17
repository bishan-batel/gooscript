//
// Created by bishan_ on 6/14/24.
//
#include "functions.hpp"

#include "ast/Statement.hpp"
#include "ast/expression/literal/Lambda.hpp"
#include "expression.hpp"

namespace goos::parser::pass::expr {
  auto lambda(TokenStream &stream) -> OptionalResult<ast::expression::Lambda> {
    ast::TokenTrace trace = stream.trace();

    if (not stream.try_consume(lexer::Keyword::FN)) {
      return OptionalResult<ast::expression::Lambda>{crab::none};
    }

    Vec<goos::meta::Identifier> parameters{};

    if (stream.try_consume(lexer::Operator::PAREN_OPEN)) {
      while (not stream.is_curr(lexer::Operator::PAREN_CLOSE)) {
        auto parameter = stream.consume_identifier();

        if (parameter.is_err()) {
          return crab::err(parameter.take_err_unchecked());
        }

        parameters.push_back(parameter.take_unchecked());

        if (stream.try_consume(lexer::Operator::COMMA)) {
          continue;
        }
        break;
      }

      if (auto err = stream.consume_operator(lexer::Operator::PAREN_CLOSE); err.is_err()) {
        return crab::err(err.take_err_unchecked());
      }
    }

    if (stream.try_consume(lexer::Operator::ARROW)) {
      MustEvalResult<ast::Expression> expr = expression(stream);

      if (expr.is_err())
        return crab::err(expr.take_err_unchecked());

      trace = trace.merge(stream.trace());

      return crab::ok(
          crab::some(crab::make_box<ast::expression::Lambda>(std::move(parameters), expr.take_unchecked(), trace)));
    }

    auto result = block(stream, false);

    if (result.is_err())
      return crab::err(result.take_err_unchecked());

    auto scope_opt = result.take_unchecked();

    if (scope_opt.is_none()) {
      return crab::err(stream.unexpected("Expected '{' after lambda."));
    }

    auto body = scope_opt.take_unchecked();

    trace = trace.merge(stream.trace());

    return crab::ok(crab::some(crab::make_box<ast::expression::Lambda>(std::move(parameters), std::move(body), trace)));
  }
} // namespace goos::parser::pass::expr
