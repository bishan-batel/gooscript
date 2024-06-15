//
// Created bany bish_ on 5/15/24.
//

#pragma once
#include <bitset>

#include "Environment.hpp"
#include "ast/Expression.hpp"
#include "ast/Statement.hpp"
#include <result.hpp>
#include <stack>

#include "data/Unit.hpp"
#include "err/RuntimeError.hpp"

namespace goos::runtime {
  enum class ControlFlowFlag : u32 {
    BREAK_LOOP = 0,
    EVAL       = 1,
    RETURN     = 2,
  };

  class Intepreter final : ast::Statement::IVisitor {
    RcMut<Environment> env{Environment::get_standard_environment()};
    std::bitset<sizeof(ControlFlowFlag) * 8> halt_flags;
    Option<Any> halt_evaluation;

    auto push_env() -> Environment&;

    auto pop_env() -> Environment&;

    [[nodiscard]] auto should_halt_control_flow() const -> bool;

    auto halt(ControlFlowFlag flag, Any value = crab::make_rc_mut<Unit>()) -> void;

    auto consume_halt_flag(ControlFlowFlag flag) -> Option<Any>;

  public:
    auto execute(const ast::Statement &statement) -> VoidResult;

    auto evaluate(const ast::Expression &expr) -> Result<Any>;

    auto visit_eval(const ast::Eval &eval) -> VoidResult override;

    auto visit_return(const ast::Return &ret) -> VoidResult override;

    auto visit_variable_declaration(const ast::VariableDeclaration &variable_declaration) -> VoidResult override;

    auto visit_array(const ast::expression::Array &array) -> Result<Any> override;

    auto visit_boolean(const ast::expression::Boolean &boolean) -> Result<Any> override;

    auto visit_decimal(const ast::expression::Decimal &decimal) -> Result<Any> override;

    auto visit_dictionary(const ast::expression::Dictionary &dictionary) -> Result<Any> override;

    auto visit_integer(const ast::expression::Integer &integer) -> Result<Any> override;

    auto visit_lambda(const ast::expression::Lambda &lambda) -> Result<Any> override;

    auto visit_nil(const ast::expression::Nil &nil) -> Result<Any> override;

    auto visit_string_literal(const ast::expression::StringLiteral &str) -> Result<Any> override;

    auto visit_unit(const ast::expression::Unit &unit) -> Result<Any> override;

    auto visit_binary(const ast::expression::Binary &binary) -> Result<Any> override;

    auto visit_function_call(
      const ast::expression::FunctionCall &function_call
    ) -> Result<Any> override;

    auto visit_identifier_binding(
      const ast::expression::IdentifierBinding &identifier
    ) -> Result<Any> override;

    auto visit_unary(const ast::expression::Unary &unary) -> Result<Any> override;

    auto visit_if(const ast::expression::If &if_expr) -> Result<Any> override;

    auto visit_scope(const ast::expression::ScopeBlock &scope) -> Result<Any> override;

    auto visit_while(const ast::expression::While &while_expr) -> Result<Any> override;

    template<typename T> requires std::is_base_of_v<Value, T>
    static auto ok(RcMut<T> any) -> Result<Any>;
  };

  template<typename T> requires std::is_base_of_v<Value, T>
  auto Intepreter::ok(RcMut<T> any) -> Result<Any> {
    return Result<Any>{any};
  }
}
