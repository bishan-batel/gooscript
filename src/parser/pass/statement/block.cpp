//
// Created by bishan_ on 5/4/24.
//

#include "block.hpp"

#include <error.hpp>
#include <token/Operator.hpp>

#include "parser/pass/statement/statement.hpp"

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
    }

    // make a scope block & cast it to a basic statement;
    Box<ast::Statement> block_statement{crab::make_box<ast::expression::ScopeBlock>(std::move(statements))};
    return crab::ok(crab::some(std::move(block_statement)));
  }

  MustEvalResult<ast::expression::ScopeBlock> block_top_level(TokenStream &stream) {
    Vec<Box<ast::Statement>> statements{};

    while (not stream.is_eof()) {
      auto result = statement(stream);
      if (result.is_err()) return crab::err(result.take_err_unchecked());
      statements.push_back(result.take_unchecked());
    }

    return crab::ok(crab::make_box<ast::expression::ScopeBlock>(std::move(statements)));
  }
}
