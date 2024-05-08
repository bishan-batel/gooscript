//
// Created by bishan_ on 5/8/24.
//

#pragma once
#include "ast/expression/IdentifierBinding.hpp"
#include "ast/expression/literal/Decimal.hpp"
#include "ast/expression/literal/Integer.hpp"
#include "ast/expression/literal/Nil.hpp"
#include "ast/expression/literal/StringLiteral.hpp"
#include "ast/expression/literal/Unit.hpp"
#include "parser/TokenStream.hpp"

namespace goos::parser::pass::expr {
  auto decimal(TokenStream &stream) -> OptionalResult<ast::expression::Decimal>;

  auto integer(TokenStream &stream) -> OptionalResult<ast::expression::Integer>;

  auto string(TokenStream &stream) -> OptionalResult<ast::expression::StringLiteral>;

  auto unit(TokenStream &stream) -> OptionalResult<ast::expression::Unit>;

  auto null(TokenStream &stream) -> OptionalResult<ast::expression::Nil>;

  auto identifier_binding(TokenStream &stream) -> OptionalResult<ast::expression::IdentifierBinding>;
}
