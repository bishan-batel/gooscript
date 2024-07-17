//
// Created by bishan_ on 6/27/24.
//

#pragma once
#include <rc.hpp>
#include "Builder.hpp"
#include "ast/Statement.hpp"
#include "err/Error.hpp"

namespace goos::codegen {
  class Compiler final : public ast::Statement::IVisitor {
    Builder builder;
    // Rc<Enviornment> env = Enviornment::standard_enviornment();

  public:
    explicit Compiler(WideString name);

    static auto to_ok(Value value) -> Result<std::any, Box<crab::Error>>;

    static auto to_err(Box<err::Error> value) -> Result<std::any, Box<crab::Error>>;

    [[nodiscard]] auto finalize_chunk() -> vm::Chunk;

    auto compile(const ast::Statement &statement) -> CompilerResult<unit>;

    auto compile(const ast::Expression &expr) -> CompilerResult<Value>;

    auto visit_eval(const ast::Eval &eval) -> Result<unit, Box<crab::Error>> override;

    auto visit_return(const ast::Return &ret) -> Result<unit, Box<crab::Error>> override;

    auto visit_variable_declaration(
      const ast::VariableDeclaration &variable_declaration
    ) -> Result<unit, Box<crab::Error>> override;

    auto visit_array(const ast::expression::Array &array) -> Result<std::any, Box<crab::Error>> override;

    auto visit_boolean(const ast::expression::Boolean &boolean) -> Result<std::any, Box<crab::Error>> override;

    auto visit_decimal(const ast::expression::Decimal &decimal) -> Result<std::any, Box<crab::Error>> override;

    auto visit_dictionary(const ast::expression::Dictionary &dictionary) -> Result<std::any, Box<crab::Error>> override;

    auto visit_integer(const ast::expression::Integer &integer) -> Result<std::any, Box<crab::Error>> override;

    auto visit_lambda(const ast::expression::Lambda &lambda) -> Result<std::any, Box<crab::Error>> override;

    auto visit_nil(const ast::expression::Nil &nil) -> Result<std::any, Box<crab::Error>> override;

    auto visit_string_literal(const ast::expression::StringLiteral &str) -> Result<std::any, Box<crab::Error>> override;

    auto visit_unit(const ast::expression::Unit &unit) -> Result<std::any, Box<crab::Error>> override;

    auto visit_binary(const ast::expression::Binary &binary) -> Result<std::any, Box<crab::Error>> override;

    auto visit_function_call(
      const ast::expression::FunctionCall &function_call
    ) -> Result<std::any, Box<crab::Error>> override;

    auto visit_identifier_binding(
      const ast::expression::IdentifierBinding &identifier
    ) -> Result<std::any, Box<crab::Error>> override;

    auto visit_unary(const ast::expression::Unary &unary) -> Result<std::any, Box<crab::Error>> override;

    auto visit_if(const ast::expression::If &if_expr) -> Result<std::any, Box<crab::Error>> override;

    auto visit_scope(const ast::expression::ScopeBlock &scope) -> Result<std::any, Box<crab::Error>> override;

    auto visit_while(const ast::expression::While &while_expr) -> Result<std::any, Box<crab::Error>> override;

    auto visit_property_access(
      const ast::expression::PropertyAccess &property_access
    ) -> Result<std::any, Box<crab::Error>> override;

    auto visit_array_index(
      const ast::expression::ArrayIndex &array_index
    ) -> Result<std::any, Box<crab::Error>> override;

    auto visit_match(const ast::expression::Match &match) -> Result<std::any, Box<crab::Error>> override;
  };
} // goos
