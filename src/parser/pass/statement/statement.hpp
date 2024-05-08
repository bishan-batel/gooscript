//
// Created by bishan_ on 5/5/24.
//

#pragma once
#include "../expression/expression.hpp"
#include "ast/statements/VariableDeclaration.hpp"
#include "parser/TokenStream.hpp"
#include "parser/pass/statement/block.hpp"

namespace goos::parser::pass {
  /**
   * @brief Pass for parsing any statemennt, this function must evaluate to a valid statement or an Error.
   */
  [[nodiscard]] auto statement(TokenStream &stream) -> MustEvalResult<ast::Statement>;

  /**
   * @brief Pass for parsing a variable declaration statement, this function is able to return None when non applicable
   */
  [[nodiscard]] auto variable_declare(TokenStream &stream) -> OptionalResult<ast::VariableDeclaration>;

  namespace meta {
    template<typename F> requires std::is_invocable_v<F, TokenStream&>
    auto transmute(F f) -> OptionalPass<ast::Statement> {
      return [f](TokenStream &stream) -> OptionalResult<ast::Statement> {
        auto result{f(stream)};
        if (result.is_err()) return crab::err(result.take_err_unchecked());

        auto opt = result.take_unchecked();

        if (opt.is_none()) return crab::ok<Option<Box<ast::Statement>>>(crab::none);

        return crab::ok<Option<Box<ast::Statement>>>(crab::some(Box<ast::Statement>{opt.take_unchecked()}));
      };
    }
  }

  inline static const std::array STATEMENT_PASSES{
    meta::transmute(block),
    meta::transmute(variable_declare),
  };
}
