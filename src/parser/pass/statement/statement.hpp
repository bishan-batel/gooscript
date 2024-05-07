//
// Created by bishan_ on 5/5/24.
//

#pragma once
#include "parser/TokenStream.hpp"

namespace goos::parser::pass {
  [[nodiscard]] MustEvalResult<ast::Statement> statement(TokenStream &stream);
}
