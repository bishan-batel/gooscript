//
// Created by bishan_ on 5/8/24.
//

#pragma once

#include "ast/expression/compound/If.hpp"
#include "ast/expression/compound/While.hpp"
#include "parser/TokenStream.hpp"

namespace goos::parser::pass::expr {
  auto if_condition(TokenStream &stream) -> OptionalResult<ast::expression::If>;

  auto while_loop(TokenStream &stream) -> OptionalResult<ast::expression::While>;

  auto for_loop(TokenStream &stream) -> OptionalResult<ast::expression::If>;

  auto match_statement(TokenStream &stream) -> OptionalResult<ast::expression::If>;
}
