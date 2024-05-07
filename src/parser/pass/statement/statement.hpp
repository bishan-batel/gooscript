//
// Created by bishan_ on 5/5/24.
//

#pragma once
#include "ast/statements/VariableDeclaration.hpp"
#include "parser/TokenStream.hpp"
#include "parser/pass/statement/block.hpp"

namespace goos::parser::pass {
  [[nodiscard]] MustEvalResult<ast::Statement> statement(TokenStream &stream);

  [[nodiscard]] OptionalResult<ast::VariableDeclaration> variable_declare(TokenStream &stream);

  namespace meta {
    template<typename F> requires std::is_invocable_v<F, TokenStream&>
    __always_inline OptionalPass<ast::Statement> transmute(F f) {
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
    meta::transmute(variable_declare),
    meta::transmute(block)
  };
}
