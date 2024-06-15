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
  [[nodiscard]] auto block(TokenStream &stream, bool require_do=true) -> OptionalResult<ast::expression::ScopeBlock>;

  /**
   * @brief Parses a block statement from the stream, assuming top level.
   */
  [[nodiscard]] auto block_top_level(TokenStream &stream) -> MustEvalResult<ast::expression::ScopeBlock>;

  [[nodiscard]] auto statements_list(
    TokenStream &stream,
    const std::function<bool(TokenStream &)> &try_consume_end
  ) -> Result<Box<ast::expression::ScopeBlock>>;
}
