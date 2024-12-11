//
// Created by bishan_ on 5/5/24.
//

#include "statement.hpp"

#include <error.hpp>
#include <token/Operator.hpp>
#include "ast/Statement.hpp"
#include "meta/Mutability.hpp"
#include "parser/pass/expression/expression.hpp"
#include "parser/pass/statement/block.hpp"

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

      if (auto statement = result.take_unchecked(); statement.is_some()) {
        return crab::ok(statement.take_unchecked());
      }
    }

    MustEvalResult<ast::Expression> expr = expr::expression(stream);

    if (expr.is_err()) return crab::err(expr.take_err_unchecked());

    return crab::ok<Box<ast::Statement>>(expr.take_unchecked());
  }

  auto variable_declare(TokenStream &stream) -> OptionalResult<ast::VariableDeclaration> {
    goos::meta::Mutability mutability{goos::meta::Mutability::IMMUTABLE};

    ast::TokenTrace trace{stream.curr()};

    if (stream.try_consume(lexer::Keyword::LET)) {
      mutability = goos::meta::Mutability::IMMUTABLE;
    } else if (stream.try_consume(lexer::Keyword::VAR)) {
      mutability = goos::meta::Mutability::MUTABLE;
    } else if (stream.try_consume(lexer::Keyword::CONST)) {
      mutability = goos::meta::Mutability::CONSTANT;
    } else
      return OptionalResult<ast::VariableDeclaration>{crab::none};

    Result<goos::meta::Identifier> name_result{stream.consume_identifier()};

    if (name_result.is_err()) {
      return crab::err(name_result.take_err_unchecked());
    }

    goos::meta::Identifier name{name_result.take_unchecked()};

    trace = trace.merge(stream.trace());

    Box<ast::Expression> initializer{crab::make_box<ast::expression::Unit>(trace)};
    trace = trace.merge(stream.trace());

    if (stream.try_consume(lexer::Operator::ASSIGN)) {
      MustEvalResult<ast::Expression> result = expr::expression(stream);
      if (result.is_err()) return crab::err(result.take_err_unchecked());
      initializer = result.take_unchecked();
    }

    trace = trace.merge(ast::TokenTrace{stream.curr()});

    return crab::ok(
        crab::some(crab::make_box<ast::VariableDeclaration>(name, mutability, std::move(initializer), trace)));
  }

  auto return_statement(TokenStream &stream) -> OptionalResult<ast::Return> {
    ast::TokenTrace trace{stream.curr()};

    if (not stream.try_consume(lexer::Keyword::RETURN)) {
      return OptionalResult<ast::Return>{crab::none};
    }

    trace = trace.merge(stream.trace());

    if (stream.is_eof()) {
      return crab::ok(crab::some(crab::make_box<ast::Return>(crab::make_box<ast::expression::Unit>(trace), trace)));
    }

    if (auto op = stream.curr().downcast<token::Operator>();
        op.is_some() and op.get_unchecked()->get_op() == lexer::Operator::CURLY_CLOSE) {
      return crab::ok(crab::some(crab::make_box<ast::Return>(crab::make_box<ast::expression::Unit>(trace), trace)));
    }

    MustEvalResult<ast::Expression> expr = expr::expression(stream);
    trace = trace.merge(stream.trace());

    if (expr.is_err()) {
      return expr.take_err_unchecked();
    }

    return crab::some(crab::make_box<ast::Return>(expr.take_unchecked(), trace));
  }

  auto fn_statement(TokenStream &stream) -> OptionalResult<ast::VariableDeclaration> {
    ast::TokenTrace trace = stream.trace();

    if (not stream.try_consume(lexer::Keyword::FN)) {
      return OptionalResult<ast::VariableDeclaration>{crab::none};
    }

    auto name_result = stream.consume_identifier();
    if (name_result.is_err()) return name_result.take_err_unchecked();
    goos::meta::Identifier name = name_result.take_unchecked();

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
      MustEvalResult<ast::Expression> expr = expr::expression(stream);

      trace = trace.merge(stream.trace());

      if (expr.is_err()) return crab::err(expr.take_err_unchecked());

      return crab::ok(crab::some(crab::make_box<ast::VariableDeclaration>(
          name,
          goos::meta::Mutability::IMMUTABLE,
          crab::make_box<ast::expression::Lambda>(std::move(parameters), expr.take_unchecked(), trace),
          trace)));
    }

    auto result = block(stream, false);

    if (result.is_err()) return crab::err(result.take_err_unchecked());

    auto scope_opt = result.take_unchecked();

    if (scope_opt.is_none()) {
      return crab::err(stream.unexpected("Expected '{' after lambda."));
    }

    auto body = scope_opt.take_unchecked();

    trace = trace.merge(stream.trace());

    return crab::ok(crab::some(crab::make_box<ast::VariableDeclaration>(
        name,
        goos::meta::Mutability::IMMUTABLE,
        crab::make_box<ast::expression::Lambda>(std::move(parameters), std::move(body), trace),
        trace)));
  }

  auto eval(TokenStream &stream) -> OptionalResult<ast::Eval> {
    ast::TokenTrace trace = stream.trace();

    if (not stream.try_consume(lexer::Keyword::EVAL)) {
      return OptionalResult<ast::Eval>{crab::none};
    }

    trace = trace.merge(stream.trace());

    if (stream.is_eof()) {
      return crab::ok(crab::some(crab::make_box<ast::Eval>(crab::make_box<ast::expression::Unit>(trace), trace)));
    }

    if (auto op = stream.curr().downcast<token::Operator>();
        op.is_some() and op.get_unchecked()->get_op() == lexer::Operator::CURLY_CLOSE) {
      return crab::ok(crab::some(crab::make_box<ast::Eval>(crab::make_box<ast::expression::Unit>(trace), trace)));
    }

    MustEvalResult<ast::Expression> expr = expr::expression(stream);
    if (expr.is_err()) return crab::err(expr.take_err_unchecked());

    trace = trace.merge(stream.curr());

    return crab::ok(crab::some(crab::make_box<ast::Eval>(expr.take_unchecked(), trace)));
  }
} // namespace goos::parser::pass
