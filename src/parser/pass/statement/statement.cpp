//
// Created by bishan_ on 5/5/24.
//

#include "statement.hpp"

#include "ast/expression/literal/Unit.hpp"
#include "ast/statements/Eval.hpp"

namespace goos::parser::pass {
  MustEvalResult<ast::Statement> statement([[maybe_unused]] TokenStream &stream) {
    if (stream.is_eof()) {
      return crab::err(stream.unexpected("Statement"));
    }

    // TODO
    return MustEvalResult<ast::Statement>(
      crab::make_box<ast::Eval>(crab::make_box<ast::expression::Unit>())
    );
  }
}
