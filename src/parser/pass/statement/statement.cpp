//
// Created by bishan_ on 5/5/24.
//

#include "statement.hpp"

namespace goos::parser::pass {
  OptionalResult<ast::Statement> statement([[maybe_unused]] TokenStream &stream) {
    // TODO
    return OptionalResult<ast::Statement>(crab::none);
  }
}
