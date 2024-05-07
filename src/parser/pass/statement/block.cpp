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
  OptionalResult<ast::Statement> block(TokenStream &stream) {
    if (not stream.try_consume(lexer::Operator::CURLY_OPEN)) {
      return OptionalResult<ast::Statement>(crab::none);
    }
    Vec<Box<ast::Statement>> statements{};

    while (not stream.try_consume(lexer::Operator::CURLY_CLOSE)) {
      auto result = statement(stream);
      // early return if error
      if (result.is_err()) return crab::err(result.take_err_unchecked());
      statements.push_back(result.take_unchecked());

      if (auto err = stream.consume_operator(lexer::Operator::SEMICOLON); err.is_err()) {
        return crab::err(err.take_err_unchecked());
      }
      // TODO evaluation steps
    }

    // make a scope block & cast it to a basic statement;
    Box<ast::Statement> block_statement{crab::make_box<ast::expression::ScopeBlock>(std::move(statements))};
    return crab::ok(crab::some(std::move(block_statement)));
  }

  MustEvalResult<ast::expression::ScopeBlock> block_top_level(TokenStream &stream) {
    Vec<Box<ast::Statement>> statements{};

    Box<ast::Eval> eval{crab::make_box<ast::Eval>(crab::make_box<ast::expression::Unit>())};

    while (not stream.is_eof()) {
      auto result = statement(stream);
      if (result.is_err()) return crab::err(result.take_err_unchecked());

      statements.push_back(result.take_unchecked());

      if (auto err = stream.consume_operator(lexer::Operator::SEMICOLON); err.is_err()) {
        return crab::err(err.take_err_unchecked());
      }
    }

    if (not stream.is_eof()) {
      return crab::err(stream.error<err::ExpectedToken>("EOF", stream.curr().clone()));
    }

    return crab::ok(crab::make_box<ast::expression::ScopeBlock>(std::move(statements), std::move(eval)));
  }
}
