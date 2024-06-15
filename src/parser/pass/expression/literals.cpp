//
// Created by bishan_ on 5/8/24.
//
#include "literals.hpp"

#include "expression.hpp"
#include "token/Decimal.hpp"
#include "token/Identifier.hpp"
#include "token/Integer.hpp"
#include "token/StringLiteral.hpp"

namespace goos::parser::pass::expr {
  auto grouping(TokenStream &stream) -> OptionalResult<ast::Expression> {
    if (not stream.try_consume(lexer::Operator::PAREN_OPEN)) return OptionalResult<ast::Expression>{crab::none};

    auto expr = expression(stream);
    if (expr.is_err()) return crab::err(expr.take_err_unchecked());

    if (auto err = stream.consume_operator(lexer::Operator::PAREN_CLOSE); err.is_err()) {
      return crab::err(err.take_err_unchecked());
    }

    return crab::ok(crab::some(expr.take_unchecked()));
  }

  auto decimal(TokenStream &stream) -> OptionalResult<ast::expression::Decimal> {
    if (auto decimal = stream.try_consume<token::Decimal>()) {
      const f64 number{decimal.take_unchecked()->get_number()};
      return crab::ok(crab::some(crab::make_box<ast::expression::Decimal>(number)));
    }

    return crab::ok<Option<Box<ast::expression::Decimal>>>(crab::none);
  }

  auto integer(TokenStream &stream) -> OptionalResult<ast::expression::Integer> {
    if (auto decimal = stream.try_consume<token::Integer>()) {
      const i64 number{decimal.take_unchecked()->get_number()};
      return crab::ok(crab::some(crab::make_box<ast::expression::Integer>(number)));
    }

    return crab::ok<Option<Box<ast::expression::Integer>>>(crab::none);
  }

  auto string(TokenStream &stream) -> OptionalResult<ast::expression::StringLiteral> {
    if (auto str = stream.try_consume<token::StringLiteral>()) {
      const WideString literal{str.take_unchecked()->get_string()};
      return crab::ok(
        crab::some(
          crab::make_box<ast::expression::StringLiteral>(std::move(literal))
        )
      );
    }

    return crab::ok<Option<Box<ast::expression::StringLiteral>>>(crab::none);
  }

  auto unit(TokenStream &stream) -> OptionalResult<ast::expression::Unit> {
    if (stream.try_consume(lexer::Keyword::UNIT)) {
      return crab::ok(crab::some(crab::make_box<ast::expression::Unit>()));
    }

    return crab::ok<Option<Box<ast::expression::Unit>>>(crab::none);
  }

  auto null(TokenStream &stream) -> OptionalResult<ast::expression::Nil> {
    if (stream.try_consume(lexer::Keyword::NIL)) {
      return crab::ok(crab::some(crab::make_box<ast::expression::Nil>()));
    }
    return crab::ok<Option<Box<ast::expression::Nil>>>(crab::none);
  }

  auto boolean(TokenStream &stream) -> OptionalResult<ast::expression::Boolean> {
    if (stream.try_consume(lexer::Keyword::TRUE)) {
      return crab::ok(crab::some(crab::make_box<ast::expression::Boolean>(true)));
    }

    if (stream.try_consume(lexer::Keyword::FALSE)) {
      return crab::ok(crab::some(crab::make_box<ast::expression::Boolean>(false)));
    }

    return crab::ok<Option<Box<ast::expression::Boolean>>>(crab::none);
  }

  auto identifier_binding(TokenStream &stream) -> OptionalResult<ast::expression::IdentifierBinding> {
    if (auto identifier = stream.try_consume<token::Identifier>()) {
      auto binding =
          crab::make_box<ast::expression::IdentifierBinding>(identifier.take_unchecked()->get_identifier());
      return crab::ok(crab::some(std::move(binding)));
    }
    return crab::ok<Option<Box<ast::expression::IdentifierBinding>>>(crab::none);
  }

  auto array(TokenStream &stream) -> OptionalResult<ast::expression::Array> {
    if (not stream.try_consume(lexer::Operator::BRACKET_OPEN)) {
      return OptionalResult<ast::expression::Array>{crab::none};
    }

    Vec<Box<ast::Expression>> elements;

    while (not stream.is_curr(lexer::Operator::BRACKET_CLOSE) and not stream.is_eof()) {
      auto expr = expression(stream);
      if (expr.is_err()) return crab::err(expr.take_err_unchecked());

      elements.push_back(expr.take_unchecked());

      if (stream.try_consume(lexer::Operator::COMMA)) {
        continue;
      }

      break;
    }

    // expect a '[' at end of the array
    if (auto err = stream.consume_operator(lexer::Operator::BRACKET_CLOSE); err.is_err()) {
      return crab::err(err.take_err_unchecked());
    }

    return crab::ok(crab::some(crab::make_box<ast::expression::Array>(std::move(elements))));
  }

  auto dictionary([[maybe_unused]] TokenStream &stream) -> OptionalResult<ast::expression::Dictionary> {
    if (not stream.try_consume(lexer::Operator::CURLY_OPEN)) {
      return OptionalResult<ast::expression::Dictionary>{crab::none};
    }

    Vec<ast::expression::Dictionary::Pair> pairs;

    while (not stream.is_curr(lexer::Operator::CURLY_CLOSE) and not stream.is_eof()) {
      // Key Value
      Option<Box<ast::Expression>> key{crab::none}; {
        auto identifier{identifier_binding(stream)};
        if (identifier.is_err()) return crab::err(identifier.take_err_unchecked());

        if (auto value = identifier.take_unchecked()) {
          key = Box<ast::Expression>{value.take_unchecked()};
        }
      }

      // if not an identifier, it must be a string
      if (key.is_none()) {
        auto str{string(stream)};
        if (str.is_err()) return crab::err(str.take_err_unchecked());

        if (auto value = str.take_unchecked()) {
          key = value.take_unchecked();
        }
      }
      // check for err

      // if key is not an identifier, it must be a string
      if (key.is_none()) {
        return crab::err(stream.unexpected("An Identifier or a String"));
      }

      // Key-Value seperator is a ':' or '='
      if (auto err = stream.consume_operator(lexer::Operator::COLON, lexer::Operator::EQUALS); err.is_err()) {
        return crab::err(err.take_err_unchecked());
      }

      auto value = expression(stream);
      if (value.is_err()) return crab::err(value.take_err_unchecked());

      pairs.push_back({key.take_unchecked(), value.take_unchecked()});

      if (stream.try_consume(lexer::Operator::COMMA)) {
        continue;
      }

      break;
    }

    // expect a '[' at end of the array
    if (auto err = stream.consume_operator(lexer::Operator::BRACKET_CLOSE); err.is_err()) {
      return crab::err(err.take_err_unchecked());
    }

    return crab::ok(crab::some(crab::make_box<ast::expression::Dictionary>(std::move(pairs))));
  }
}
