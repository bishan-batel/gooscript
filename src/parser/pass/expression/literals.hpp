//
// Created by bishan_ on 5/8/24.
//

#pragma once
#include "ast/expression/IdentifierBinding.hpp"
#include "ast/expression/literal/Array.hpp"
#include "ast/expression/literal/Decimal.hpp"
#include "ast/expression/literal/Dictionary.hpp"
#include "ast/expression/literal/Integer.hpp"
#include "ast/expression/literal/Nil.hpp"
#include "ast/expression/literal/StringLiteral.hpp"
#include "ast/expression/literal/Unit.hpp"
#include "parser/TokenStream.hpp"
#include "ast/expression/literal/Boolean.hpp"

namespace goos::parser::pass::expr {
  auto grouping(TokenStream &stream) -> OptionalResult<ast::Expression>;

  auto decimal(TokenStream &stream) -> OptionalResult<ast::expression::Decimal>;

  auto integer(TokenStream &stream) -> OptionalResult<ast::expression::Integer>;

  auto string(TokenStream &stream) -> OptionalResult<ast::expression::StringLiteral>;

  auto unit(TokenStream &stream) -> OptionalResult<ast::expression::Unit>;

  auto null(TokenStream &stream) -> OptionalResult<ast::expression::Nil>;

  auto boolean(TokenStream &stream) -> OptionalResult<ast::expression::Boolean>;

  auto identifier_binding(TokenStream &stream) -> OptionalResult<ast::expression::IdentifierBinding>;

  auto array(TokenStream &stream) -> OptionalResult<ast::expression::Array>;

  auto dictionary(TokenStream &stream) -> OptionalResult<ast::expression::Dictionary>;
}
