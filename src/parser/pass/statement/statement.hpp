//
// Created by bishan_ on 5/5/24.
//

#pragma once
#include "../expression/expression.hpp"
#include "ast/statements/Return.hpp"
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

  [[nodiscard]] auto return_statement(TokenStream &stream) -> OptionalResult<ast::Return>;

  [[nodiscard]] auto eval(TokenStream &stream) -> OptionalResult<ast::Eval>;

  namespace meta {
    template<typename F>
      requires std::is_invocable_v<F, TokenStream&>
    auto transmute(F f) -> OptionalPass<ast::Statement> {
      return [f](TokenStream &stream) -> OptionalResult<ast::Statement> {
        auto result{f(stream)};
        if (result.is_err())
          return crab::err(result.take_err_unchecked());

        auto opt = result.take_unchecked();

        if (opt.is_none())
          return crab::ok<Option<Box<ast::Statement>>>(crab::none);

        return crab::ok<Option<Box<ast::Statement>>>(crab::some(Box<ast::Statement>{opt.take_unchecked()}));
      };
    }
  } // namespace meta

  inline static const std::vector STATEMENT_PASSES{
    meta::transmute([](auto &s) { return block(s); }),
    meta::transmute(variable_declare),
    meta::transmute(return_statement),
    meta::transmute(eval),
  };
} // namespace goos::parser::pass
