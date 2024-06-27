//
// Created bany bish_ on 5/15/24.
//

#pragma once
#include <bitset>

#include "ast/Expression.hpp"
#include "ast/Statement.hpp"
#include <result.hpp>

#include "Environment.hpp"
#include "data/Unit.hpp"

namespace goos::runtime {
  enum class ControlFlowFlag : u32 {
    BREAK_LOOP = 0,
    EVAL       = 1,
    RETURN     = 2,
  };

  class Environment;

  class Intepreter final : ast::Statement::IVisitor {
    RcMut<Environment> current_environment;
    std::bitset<sizeof(ControlFlowFlag) * 8> halt_flags;
    Option<Any> halt_evaluation;

  public:
    explicit Intepreter(RcMut<Environment> globals);

    explicit Intepreter();

    auto set_env(RcMut<Environment> environment) -> void;

    auto get_env() -> RcMut<Environment>;

    auto push_env(RcMut<Environment> environment) -> Environment&;

    auto push_env() -> Environment&;

    auto pop_env() -> Environment&;

    [[nodiscard]] auto should_halt_control_flow() const -> bool;

    auto halt(ControlFlowFlag flag, Any value = crab::make_rc_mut<Unit>()) -> void;

    auto consume_halt_flag(ControlFlowFlag flag) -> Option<Any>;

    [[nodiscard]] auto env() const -> Environment&;

    auto execute(const ast::Statement &statement) -> Result<unit, Box<err::Error>>;

    auto evaluate(const ast::Expression &expr) -> Result<Any, Box<err::Error>>;

    auto evaluate_shallow(const ast::Expression &expr) -> Result<Any, Box<err::Error>>;

    // auto shallow_evaluate(const ast::Expression &expr) -> Result<std::any, Box<crab::Error>;

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

    auto visit_match(const parser::pass::expr::Match &match) -> Result<std::any, Box<crab::Error>> override;
  };
}
