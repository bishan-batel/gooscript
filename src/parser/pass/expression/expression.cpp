//
// Created by bishan_ on 5/7/24.
//

#include "expression.hpp"

#include <token/Operator.hpp>

#include "ast/expression/Binary.hpp"
#include "ast/expression/FunctionCall.hpp"
#include "ast/expression/Unary.hpp"
#include "ast/expression/PropertyAccess.hpp"
#include "ast/expression/ArrayIndex.hpp"

namespace goos::parser::pass::expr {
  auto factor(TokenStream &stream) -> MustEvalResult<ast::Expression> {
    if (auto op = crab::ref::cast<token::Operator>(stream.curr()); op and is_unary(op.get_unchecked()->get_op())) {
      stream.next();
      auto operand{factor(stream)};

      if (operand.is_err()) return crab::err(operand.take_err_unchecked());

      auto unary{crab::make_box<ast::expression::Unary>(op.get_unchecked()->get_op(), operand.take_unchecked())};
      return crab::ok<Box<ast::Expression>>(std::move(unary));
    }

    for (const auto &pass: FACTOR_PASSES) {
      OptionalResult<ast::Expression> result{pass(stream)};

      if (result.is_err())
        return crab::err(result.take_err_unchecked());

      Option<Box<ast::Expression>> option{result.take_unchecked()};
      if (option.is_none()) continue;

      Box<ast::Expression> expr = option.take_unchecked();

      // Function Call
      if (stream.try_consume(lexer::Operator::PAREN_OPEN)) {
        Vec<Box<ast::Expression>> arguments;
        while (not stream.is_curr(lexer::Operator::PAREN_CLOSE)) {
          // consume parameter
          auto param{expression(stream)};
          if (param.is_err()) return crab::err(param.take_err_unchecked());
          arguments.push_back(param.take_unchecked());

          // if no comma then list is at the end
          if (not stream.try_consume(lexer::Operator::COMMA)) {
            break;
          }
        }

        if (auto err = stream.consume_operator(lexer::Operator::PAREN_CLOSE); err.is_err()) {
          return crab::err(err.take_err_unchecked());
        }

        return crab::ok(
          Box<ast::Expression>{crab::make_box<ast::expression::FunctionCall>(std::move(expr), std::move(arguments))}
        );
      }

      // just a normal expression
      return crab::ok(std::move(expr));
    }
    return crab::err(stream.unexpected("Expression"));
  }

  auto postfix_pass(TokenStream &stream, Box<ast::Expression> expr) -> MustEvalResult<ast::Expression> {
    if (stream.try_consume(lexer::Operator::DOT) or stream.try_consume(lexer::Operator::COLON)) {
      Result<goos::meta::Identifier> ident = stream.consume_identifier();
      if (ident.is_err()) return ident.take_err_unchecked();

      return postfix_pass(
        stream,
        crab::make_box<ast::expression::PropertyAccess>(
          std::move(expr),
          ident.take_unchecked()
        )
      );
    }

    if (stream.try_consume(lexer::Operator::BRACKET_OPEN)) {
      MustEvalResult<ast::Expression> index = expression(stream);
      if (index.is_err()) return index.take_err_unchecked();

      if (auto err = stream.consume_operator(lexer::Operator::BRACKET_CLOSE); err.is_err()) {
        return err.take_err_unchecked();
      }

      return postfix_pass(
        stream,
        crab::make_box<ast::expression::ArrayIndex>(
          std::move(expr),
          index.take_unchecked()
        )
      );
    }
    return expr;
  }

  auto postfix(TokenStream &stream) -> MustEvalResult<ast::Expression> {
    MustEvalResult<ast::Expression> expr_result = factor(stream);
    if (expr_result.is_err()) return expr_result.take_err_unchecked();
    return postfix_pass(stream, expr_result.take_unchecked());
  }

  auto consume_binary_expression(TokenStream &stream, const usize op_index) -> MustEvalResult<ast::Expression> {
    if (op_index == 0) {
      return postfix(stream);
    }
    const auto parse{
      [&stream, op_index] { return consume_binary_expression(stream, op_index - 1); }
    };

    // [[maybe_unused]]
    // usize begin{stream.get_position()};

    // parse left hand side
    auto result{parse()};
    if (result.is_err())
      return crab::err(result.take_err_unchecked());

    Box<ast::Expression> lhs{result.take_unchecked()};

    // Valid Operators
    const Set<lexer::Operator> &operators = ORDER_OF_OPERATIONS.at(op_index);

    while (not stream.is_eof()) {
      auto op_option{crab::ref::cast<token::Operator>(stream.curr())};
      if (op_option.is_none())
        break;

      const lexer::Operator op{op_option.take_unchecked()->get_op()};

      if (not operators.contains(op))
        break;

      // skip over operator
      stream.next();

      debug_assert(lexer::is_binary(op), "Invalid Operator Table, Operator is not Binary");

      MustEvalResult<ast::Expression> rhs{parse()};

      if (rhs.is_err())
        return crab::err(rhs.take_err_unchecked());

      lhs = crab::make_box<ast::expression::Binary>(
        std::move(lhs),
        op,
        rhs.take_unchecked()
      );
    }

    return crab::ok(std::move(lhs));
  }

  auto expression(TokenStream &stream) -> MustEvalResult<ast::Expression> {
    return consume_binary_expression(stream, ORDER_OF_OPERATIONS.size() - 1);
  }
}
