//
// Created by bishan_ on 5/5/24.
//

#include "statement.hpp"

#include "parser/pass/statement/block.hpp"
#include <error.hpp>

namespace goos::parser::pass {
  MustEvalResult<ast::Statement> statement([[maybe_unused]] TokenStream &stream) {
    if (stream.is_eof()) {
      return crab::err(stream.unexpected("Statement"));
    }

    // go through each statement passes
    for (const auto &pass: STATEMENT_PASSES) {
      auto result = pass(stream);

      // error propagation
      if (result.is_err()) {
        return crab::err(result.take_err_unchecked());
      }

      if (auto statement = result.take_unchecked()) {
        return crab::ok(statement.take_unchecked());
      }
    }

    return crab::err(stream.error<err::ExpectedToken>("Statement", stream.curr().clone()));
  }

  OptionalResult<ast::VariableDeclaration> variable_declare(TokenStream &stream) {
    goos::meta::Mutability mutability;

    if (stream.try_consume(lexer::Keyword::LET)) {
      mutability = goos::meta::Mutability::IMMUTABLE;
    } else if (stream.try_consume(lexer::Keyword::VAR)) {
      mutability = goos::meta::Mutability::MUTABLE;
    } else return OptionalResult<ast::VariableDeclaration>{crab::none};

    Result<WideString> name_result{stream.consume_identifier()};
    if (name_result.is_err()) return crab::err(name_result.take_err_unchecked());
    WideString name{name_result.take_unchecked()};

    if (stream.is_curr(lexer::Operator::ASSIGN)) {
      crab::todo<unit>();
    }

    return crab::ok(crab::some(crab::make_box<ast::VariableDeclaration>(name, mutability)));
  }
}
