//
// Created by bishan_ on 5/5/24.
//

#include "statement.hpp"

#include "parser/pass/statement/block.hpp"
#include <error.hpp>
#include <token/Operator.hpp>

namespace goos::parser::pass {
  auto statement([[maybe_unused]] TokenStream &stream) -> MustEvalResult<ast::Statement> {
    if (stream.is_eof()) {
      return crab::err(stream.unexpected("Statement"));
    }

    // go through each statement passes
    for (const auto &pass: STATEMENT_PASSES) {
      OptionalResult<ast::Statement> result{pass(stream)};

      // error propagation
      if (result.is_err()) {
        return crab::err(result.take_err_unchecked());
      }

      if (auto statement = result.take_unchecked()) {
        return crab::ok(statement.take_unchecked());
      }
    }

    MustEvalResult<ast::Expression> expr = expr::expression(stream);

    if (expr.is_err()) return crab::err(expr.take_err_unchecked());

    return crab::ok<Box<ast::Statement>>(expr.take_unchecked());
  }

  auto variable_declare(TokenStream &stream) -> OptionalResult<ast::VariableDeclaration> {
    goos::meta::Mutability mutability;

    if (stream.try_consume(lexer::Keyword::LET)) {
      mutability = goos::meta::Mutability::IMMUTABLE;
    } else if (stream.try_consume(lexer::Keyword::VAR)) {
      mutability = goos::meta::Mutability::MUTABLE;
    } else if (stream.try_consume(lexer::Keyword::CONST)) {
      mutability = goos::meta::Mutability::CONSTANT;
    } else return OptionalResult<ast::VariableDeclaration>{crab::none};

    Result<goos::meta::Identifier> name_result{stream.consume_identifier()};

    if (name_result.is_err()) {
      return crab::err(name_result.take_err_unchecked());
    }

    WideString name{name_result.take_unchecked()};

    Box<ast::Expression> initializer{crab::make_box<ast::expression::Unit>()};

    if (stream.try_consume(lexer::Operator::ASSIGN)) {
      MustEvalResult<ast::Expression> result = expr::expression(stream);
      if (result.is_err()) return crab::err(result.take_err_unchecked());
      initializer = result.take_unchecked();
    }

    return crab::ok(crab::some(crab::make_box<ast::VariableDeclaration>(name, mutability, std::move(initializer))));
  }

  auto return_statement(TokenStream &stream) -> OptionalResult<ast::Return> {
    if (not stream.try_consume(lexer::Keyword::RETURN)) {
      return OptionalResult<ast::Return>{crab::none};
    }

    if (stream.is_eof()) {
      return crab::ok(crab::some(crab::make_box<ast::Return>(crab::make_box<ast::expression::Unit>())));
    }

    if (auto op = crab::ref::cast<token::Operator>(stream.curr());
      op and op.get_unchecked()->get_op() == lexer::Operator::CURLY_CLOSE) {
      return crab::ok(crab::some(crab::make_box<ast::Return>(crab::make_box<ast::expression::Unit>())));
    }

    MustEvalResult<ast::Expression> expr = expr::expression(stream);
    if (expr.is_err()) return crab::err(expr.take_err_unchecked());

    return crab::ok(crab::some(crab::make_box<ast::Return>(expr.take_unchecked())));
  }

  auto eval(TokenStream &stream) -> OptionalResult<ast::Eval> {
    if (not stream.try_consume(lexer::Keyword::EVAL)) {
      return OptionalResult<ast::Eval>{crab::none};
    }

    if (stream.is_eof()) {
      return crab::ok(crab::some(crab::make_box<ast::Eval>(crab::make_box<ast::expression::Unit>())));
    }

    if (auto op = crab::ref::cast<token::Operator>(stream.curr());
      op and op.get_unchecked()->get_op() == lexer::Operator::CURLY_CLOSE) {
      return crab::ok(crab::some(crab::make_box<ast::Eval>(crab::make_box<ast::expression::Unit>())));
    }

    MustEvalResult<ast::Expression> expr = expr::expression(stream);
    if (expr.is_err()) return crab::err(expr.take_err_unchecked());

    return crab::ok(crab::some(crab::make_box<ast::Eval>(expr.take_unchecked())));
  }
}
