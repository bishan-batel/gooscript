//
// Created by bishan_ on 5/4/24.
//

#pragma once
#include "ast/expression/compound/ScopeBlock.hpp"
#include "parser/TokenStream.hpp"

namespace goos::parser::pass {
  [[nodiscard]] OptionalResult<ast::Statement> block(TokenStream &stream);

  [[nodiscard]] MustEvalResult<ast::Statement> block_no_curly(TokenStream &stream);
}
