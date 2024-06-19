//
// Created by bishan_ on 5/4/24.
//

#include "block.hpp"

#include <error.hpp>
#include <token/Operator.hpp>

#include "ast/expression/literal/Unit.hpp"
#include "parser/pass/statement/statement.hpp"

namespace goos::parser::pass {
  auto block(TokenStream &stream, const bool require_do) -> OptionalResult<ast::expression::ScopeBlock> {
    if (require_do and not stream.try_consume(lexer::Keyword::DO)) {
      return {crab::none};
    }

    if (auto err = stream.consume_operator(lexer::Operator::CURLY_OPEN); err.is_err()) {
      return crab::err(err.take_err_unchecked());
    }

    auto list{
      statements_list(
        stream,
        [](TokenStream &s) { return s.try_consume(lexer::Operator::CURLY_CLOSE); }
      )
    };

    // if failed to read propogate error
    if (list.is_err()) return crab::err(list.take_err_unchecked());

    return crab::ok(crab::some(list.take_unchecked()));
  }

  auto block_top_level(TokenStream &stream) -> MustEvalResult<ast::expression::ScopeBlock> {
    auto list{
      statements_list(
        stream,
        [](const TokenStream &s) { return s.is_eof(); }
      )
    };

    // if failed to read propogate error
    if (list.is_err()) return crab::err(list.take_err_unchecked());

    return crab::ok(list.take_unchecked());
  }

  // TODO fix missing error when there is no ending delimeter
  auto statements_list(
    TokenStream &stream,
    const std::function<bool(TokenStream &)> &try_consume_end
  ) -> Result<Box<ast::expression::ScopeBlock>> {
    // Block must begin with a open curly brace

    Vec<Box<ast::Statement>> statements{};
    Box<ast::Expression> eval{crab::make_box<ast::expression::Unit>()};

    while (not try_consume_end(stream) and not stream.is_eof()) {
      // const auto &start_tok = stream.curr();

      auto result = statement(stream);
      // early return if error
      if (result.is_err()) {
        return crab::err(result.take_err_unchecked());
      }

      Box<ast::Statement> statement = result.take_unchecked();

      // optionally consume a semicolon / denoter for no evaluation
      if (stream.try_consume(lexer::Operator::SEMICOLON) or not try_consume_end(stream)) {
        statements.push_back(std::move(statement));
        continue;
      }

      // if last statement is an expression, it can be used as evaluation
      if (Option<Ref<ast::Expression>> expr = statement->try_as<ast::Expression>()) {
        eval = expr.take_unchecked()->clone_expr();
      }

      break;
    }

    return crab::ok(crab::make_box<ast::expression::ScopeBlock>(std::move(statements), std::move(eval)));
  }
}
