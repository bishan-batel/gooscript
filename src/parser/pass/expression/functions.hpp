//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include "parser/TokenStream.hpp"
#include "ast/expression/literal/Lambda.hpp"

namespace goos::parser::pass::expr {
  auto lambda(TokenStream &stream) -> OptionalResult<ast::expression::Lambda>;
}
