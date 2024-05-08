//
// Created by bishan_ on 5/4/24.
//

#include "block.hpp"

#include <error.hpp>
#include <token/Operator.hpp>

#include "ast/expression/literal/Unit.hpp"
#include "parser/pass/statement/statement.hpp"

// TODO final block evaluation:

namespace goos::parser::pass {
  auto block(TokenStream &stream) -> OptionalResult<ast::Statement> {
    // Block must begin with a open curly brace
    if (not stream.try_consume(lexer::Keyword::DO)) {
      return OptionalResult<ast::Statement>(crab::none);
    }

    if (auto err = stream.consume_operator(lexer::Operator::CURLY_OPEN); err.is_err()) {
      return crab::err(err.take_err_unchecked());
    }

    Vec<Box<ast::Statement>> statements{};
    Box<ast::Eval> eval{crab::make_box<ast::Eval>(crab::make_box<ast::expression::Unit>())};

    while (not stream.try_consume(lexer::Operator::CURLY_CLOSE) and not stream.is_eof()) {
      const auto &start_tok = stream.curr();

      auto result = statement(stream);
      // early return if error
      if (result.is_err()) return crab::err(result.take_err_unchecked());

      // semicolon implies next / no evaluation
      if (stream.try_consume(lexer::Operator::SEMICOLON)) {
        statements.push_back(result.take_unchecked());
        continue;
      }

      // No semi colon, this must mean evaluation

      // Require closing right after
      if (auto err = stream.consume_operator(lexer::Operator::CURLY_CLOSE); err.is_err()) {
        return crab::err(err.take_err_unchecked());
      }

      auto statement_box = result.take_unchecked();

      // if last statement is an expression, it can be used as evaluation
      if (Option<Ref<ast::Expression>> expr = statement_box->try_as<ast::Expression>()) {
        eval = crab::make_box<ast::Eval>(expr.take_unchecked()->clone_expr());
        break;
      }

      return crab::err(stream.error<err::ExpectedExpression>(start_tok.clone(), stream.curr().clone()));
    }

    // make a scope block & cast it to a basic statement;
    Box<ast::Statement> block_statement{
      crab::make_box<ast::expression::ScopeBlock>(std::move(statements), std::move(eval))
    };
    return crab::ok(crab::some(std::move(block_statement)));
  }

  auto block_top_level(TokenStream &stream) -> MustEvalResult<ast::expression::ScopeBlock> {
    Vec<Box<ast::Statement>> statements{};

    Box<ast::Eval> eval{crab::make_box<ast::Eval>(crab::make_box<ast::expression::Unit>())};

    while (not stream.is_eof()) {
      const auto &start_tok = stream.curr();

      auto result = statement(stream);
      // early return if error
      if (result.is_err()) return crab::err(result.take_err_unchecked());

      // semicolon implies next / no evaluation
      if (stream.try_consume(lexer::Operator::SEMICOLON)) {
        statements.push_back(result.take_unchecked());
        continue;
      }

      // No semi colon, this must mean evaluation

      // Require closing right after
      if (not stream.try_consume<token::EndOfFile>()) {
        return crab::err(
          stream.error<err::ExpectedToken>("Expected end of file after evaluation statement.", stream.curr().clone())
        );
      }

      auto statement_box = result.take_unchecked();

      // if last statement is an expression, it can be used as evaluation
      if (Option<Ref<ast::Expression>> expr = statement_box->try_as<ast::Expression>()) {
        eval = crab::make_box<ast::Eval>(expr.take_unchecked()->clone_expr());
        break;
      }

      return crab::err(stream.error<err::ExpectedExpression>(start_tok.clone(), stream.curr().clone()));
    }

    // make a scope block & cast it to a basic statement;
    return crab::ok(crab::make_box<ast::expression::ScopeBlock>(std::move(statements), std::move(eval)));
  }
}
