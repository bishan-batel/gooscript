//
// Created by bishan_ on 5/4/24.
//

#pragma once
#include "ast/expression/compound/ScopeBlock.hpp"
#include "parser/TokenStream.hpp"

namespace goos::parser::pass {
  /**
   * @brief Attempts to parse a block statement from a stream, if applicable
   */
  [[nodiscard]] auto block(TokenStream &stream) -> OptionalResult<ast::Statement>;

  /**
   * @brief Parses a block statement from the stream, assuming top level.
   */
  [[nodiscard]] auto block_top_level(TokenStream &stream) -> MustEvalResult<ast::expression::ScopeBlock>;
}
