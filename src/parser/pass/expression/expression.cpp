//
// Created by bishan_ on 5/7/24.
//

#include "expression.hpp"

#include <token/Operator.hpp>

#include "ast/expression/Binary.hpp"
#include "ast/expression/Unary.hpp"
#include "ast/expression/literal/Decimal.hpp"
#include "ast/expression/literal/Unit.hpp"

namespace goos::parser::pass::expr {
  auto factor(TokenStream &stream) -> MustEvalResult<ast::Expression> {
    for (const auto &pass: FACTOR_PASSES) {
      OptionalResult<ast::Expression> result{pass(stream)};
      if (result.is_err()) return crab::err(result.take_err_unchecked());

      if (Option<Box<ast::Expression>> expr{result.take_unchecked()}) {
        return crab::ok(expr.take_unchecked());
      }
    }
    return crab::err(stream.unexpected("Unknown"));
  }

  auto consume_binary_expression(TokenStream &stream, const usize op_index) -> MustEvalResult<ast::Expression> {
    if (op_index == 0) {
      return factor(stream);
    }
    const auto parse{
      [&stream, op_index] { return consume_binary_expression(stream, op_index - 1); }
    };

    // [[maybe_unused]]
    // usize begin{stream.get_position()};

    // parse left hand side
    auto result{parse()};
    if (result.is_err()) return crab::err(result.take_err_unchecked());

    Box<ast::Expression> lhs{result.take_unchecked()};

    // Valid Operators
    const Set<lexer::Operator> &operators = ORDER_OF_OPERATIONS.at(op_index);

    while (not stream.is_eof()) {
      auto op_option{crab::ref::cast<token::Operator>(stream.curr())};
      if (op_option.is_none()) break;

      const lexer::Operator op{op_option.take_unchecked()->get_op()};

      if (not operators.contains(op)) break;

      // skip over operator
      stream.next();

      debug_assert(lexer::is_binary(op), "Invalid Operator Table, Operator is not Binary");

      MustEvalResult<ast::Expression> rhs{parse()};

      if (rhs.is_err()) return crab::err(rhs.take_err_unchecked());

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
