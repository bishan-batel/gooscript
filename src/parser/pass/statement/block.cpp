//
// Created by bishan_ on 5/4/24.
//

#include "block.hpp"

#include <error.hpp>
#include <token/Operator.hpp>

namespace goos::parser::pass {
  OptionalResult<ast::Statement> block(TokenStream &stream) {
    if (not stream.try_consume(lexer::Operator::CURLY_OPEN)) {
      return OptionalResult<ast::Statement>(crab::none);
    }

    Vec<Box<ast::Statement>> statements{};

    while (auto statement = false) {}

    Box<ast::Statement> block_statement = crab::make_box<ast::expression::ScopeBlock>(std::move(statements));

    return OptionalResult<ast::Statement>(crab::some(std::move(block_statement)));
  }

  MustEvalResult<ast::Statement> block_no_curly(TokenStream &stream) {
    return MustEvalResult<ast::Statement>{stream.error<err::ExpectedToken>("", stream.curr().clone())};
  }
}
