//
// Created by bishan_ on 6/27/24.
//

#include "Compiler.hpp"

#include "ast/Expression.hpp"
#include "ast/expression/Binary.hpp"
#include "ast/expression/FunctionCall.hpp"
#include "ast/expression/IdentifierBinding.hpp"
#include "ast/expression/compound/If.hpp"
#include "ast/expression/compound/Match.hpp"
#include "ast/expression/compound/ScopeBlock.hpp"
#include "ast/expression/compound/While.hpp"
#include "ast/expression/literal/Boolean.hpp"
#include "ast/expression/literal/Decimal.hpp"
#include "ast/expression/literal/Integer.hpp"
#include "ast/expression/literal/Nil.hpp"
#include "ast/statements/VariableDeclaration.hpp"
#include "codegen/Enviornment.hpp"
#include "codegen/err/Error.hpp"
#include "lexer/Operator.hpp"
#include "meta/Mutability.hpp"
#include "vm/Instruction.hpp"

namespace goos::codegen {
  Compiler::Compiler(WideString name) : builder{Builder{std::move(name)}} {}

  auto Compiler::to_ok(Value value) -> Result<std::any, Box<crab::Error>> { return std::any{value}; }

  auto Compiler::to_err(Box<err::Error> value) -> Result<std::any, Box<crab::Error>> {
    return Box<crab::Error>{std::move(value)};
  }

  auto Compiler::finalize_chunk() -> vm::Chunk { return builder.finalize_chunk(); }

  auto Compiler::compile(const ast::Statement &statement) -> CompilerResult<unit> {
    Result<unit, Box<crab::Error>> err = statement.accept(*this);

    if (statement.try_as<ast::Expression>().is_some()) {
      builder.write<vm::op::Code::POP>(statement.token_trace());
    }

    return err.map_err([](Box<crab::Error> error) {
      return Box<err::Error>::wrap_unchecked(&dynamic_cast<err::Error &>(*decltype(error)::unwrap(std::move(error))));
    });
  }

  auto Compiler::compile(const ast::Expression &expr) -> CompilerResult<Value> {
    return expr.accept_expr(*this)
        .map([](auto c) { return std::any_cast<Value>(c); })
        .map_err([](Box<crab::Error> error) {
          return Box<err::Error>::wrap_unchecked(
              &dynamic_cast<err::Error &>(*decltype(error)::unwrap(std::move(error))));
        });
  }

  auto Compiler::visit_eval([[maybe_unused]] const ast::Eval &eval) -> Result<unit, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto Compiler::visit_return([[maybe_unused]] const ast::Return &ret) -> Result<unit, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto Compiler::visit_variable_declaration([[maybe_unused]] const ast::VariableDeclaration &variable_declaration)
      -> Result<unit, Box<crab::Error>> {

    // build initial value
    CompilerResult<Value> result = compile(variable_declaration.get_initializer());

    if (result.is_err()) {
      return to_err(result.take_err_unchecked()).map([](auto) { return unit{}; });
    }

    const usize stack_pos = builder.get_stack_tracker();

    // register value
    // TODO: proper error handling for duplicate names
    const auto err = builder.current_env()->register_variable(
        variable_declaration.get_name(), stack_pos, variable_declaration.get_mutability());

    return unit{};
  }

  auto
  Compiler::visit_array([[maybe_unused]] const ast::expression::Array &array) -> Result<std::any, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto Compiler::visit_boolean(const ast::expression::Boolean &boolean) -> Result<std::any, Box<crab::Error>> {
    return to_ok(builder.write_push_constant(boolean.get_state(), boolean.token_trace()));
  }

  auto Compiler::visit_decimal(const ast::expression::Decimal &decimal) -> Result<std::any, Box<crab::Error>> {
    return to_ok(builder.write_push_constant(decimal.get_number(), decimal.token_trace()));
  }

  auto Compiler::visit_dictionary([[maybe_unused]] const ast::expression::Dictionary &dictionary)
      -> Result<std::any, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto Compiler::visit_integer(const ast::expression::Integer &integer) -> Result<std::any, Box<crab::Error>> {
    return to_ok(builder.write_push_constant(integer.get_number(), integer.token_trace()));
  }

  auto
  Compiler::visit_lambda([[maybe_unused]] const ast::expression::Lambda &lambda) -> Result<std::any, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto Compiler::visit_nil(const ast::expression::Nil &nil) -> Result<std::any, Box<crab::Error>> {
    return to_ok(builder.write_push_constant(vm::Nil::val, nil.token_trace()));
  }

  auto Compiler::visit_string_literal([[maybe_unused]] const ast::expression::StringLiteral &str)
      -> Result<std::any, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto Compiler::visit_unit(const ast::expression::Unit &unit) -> Result<std::any, Box<crab::Error>> {
    return to_ok(builder.write_push_constant(unit::val, unit.token_trace()));
  }

  auto
  Compiler::visit_binary([[maybe_unused]] const ast::expression::Binary &binary) -> Result<std::any, Box<crab::Error>> {
    debug_assert(lexer::is_binary(binary.get_op()), "Invalid Binary Operator");

    if (binary.get_op() == lexer::Operator::ASSIGN) {
      // TODO: error handling
      //
      const ast::expression::IdentifierBinding &ident =
          binary.get_lhs().try_as<ast::expression::IdentifierBinding>().take_unchecked();

      return compile(binary.get_rhs())
          .map([&](auto) {
            Enviornment::Variable var = builder.current_env()->find_variable(ident.get_identifier()).take_unchecked();
            debug_assert(
                var.mutability == meta::Mutability::MUTABLE,
                "Cannot reassign immutable variable (try 'var' instead of 'let')");

            const Value res = builder.write_set(builder.get_stack_tracker() - var.stack_offset, binary.token_trace());
            builder.write<vm::op::Code::UNIT>(binary.token_trace());

            return std::any{res};
          })
          .map_err([](auto err) { return Box<crab::Error>{std::move(err)}; });

      return to_ok(unit{});
    }

    return crab::fallible<Box<err::Error>>(
               [&] { return compile(binary.get_lhs()); }, [&] { return compile(binary.get_rhs()); })
        .and_then([&](std::tuple<Value, Value> operands) -> CompilerResult<Value> {
          const auto [lhs, rhs] = operands;

          switch (binary.get_op()) {
            // TODO
            case lexer::Operator::ASSIGN:
              break;
            case lexer::Operator::MOD_ASSIGN:
              break;
            case lexer::Operator::MUL_ASSIGN:
              break;
            case lexer::Operator::DIV_ASSIGN:
              break;
            case lexer::Operator::ADD_ASSIGN:
              break;
            case lexer::Operator::SUB_ASSIGN:
              break;
            case lexer::Operator::BIT_OR_ASSIGN:
              break;
            case lexer::Operator::BIT_AND_ASSIGN:
              break;

            case lexer::Operator::XOR:
              return builder.write<vm::op::Code::BIT_XOR>(binary.token_trace());
            case lexer::Operator::BIT_OR:
              return builder.write<vm::op::Code::BIT_OR>(binary.token_trace());
            case lexer::Operator::BIT_AND:
              return builder.write<vm::op::Code::BIT_AND>(binary.token_trace());
            case lexer::Operator::AND:
              return builder.write<vm::op::Code::AND>(binary.token_trace());
            case lexer::Operator::OR:
              return builder.write<vm::op::Code::OR>(binary.token_trace());
            case lexer::Operator::NOT:
              return builder.write<vm::op::Code::NOT>(binary.token_trace());
            case lexer::Operator::EQUALS:
              return builder.write<vm::op::Code::EQUALS>(binary.token_trace());

            case lexer::Operator::NOT_EQUALS: {
              builder.write<vm::op::Code::EQUALS>(binary.token_trace());
              return builder.write<vm::op::Code::NOT>(binary.token_trace());
            }

            case lexer::Operator::GREATER:
              return builder.write<vm::op::Code::GREATER_THAN>(binary.token_trace());
            case lexer::Operator::LESS:
              return builder.write<vm::op::Code::LESS_THAN>(binary.token_trace());
            case lexer::Operator::GREATER_OR_EQUALS:
              return builder.write<vm::op::Code::GREATER_OR_EQUAL_THAN>(binary.token_trace());
            case lexer::Operator::LESS_OR_EQUALS:
              return builder.write<vm::op::Code::LESS_OR_EQUAL_THAN>(binary.token_trace());
            case lexer::Operator::SHIFT_LEFT:
              return builder.write<vm::op::Code::SHIFT_LEFT>(binary.token_trace());
            case lexer::Operator::SHIFT_RIGHT:
              return builder.write<vm::op::Code::SHIFT_RIGHT>(binary.token_trace());
            case lexer::Operator::ADD:
              return builder.write<vm::op::Code::ADD>(binary.token_trace());
            case lexer::Operator::SUB:
              return builder.write<vm::op::Code::SUBTRACT>(binary.token_trace());
            case lexer::Operator::MUL:
              return builder.write<vm::op::Code::MULIPLY>(binary.token_trace());
            case lexer::Operator::DIV:
              return builder.write<vm::op::Code::DIVIDE>(binary.token_trace());
            case lexer::Operator::MOD:
              return builder.write<vm::op::Code::MODULO>(binary.token_trace());

              // case lexer::Operator::IN:
              //   break;

            default:
              return Value{unit{}};
          }

          // TODO error for missing case
          return Value{unit{}};
        })
        .map([](auto v) { return std::any{v}; })
        .map_err([](auto err) { return Box<crab::Error>{std::move(err)}; });

    // TODO
    return to_ok(unit{});
  }

  auto Compiler::visit_function_call([[maybe_unused]] const ast::expression::FunctionCall &function_call)
      -> Result<std::any, Box<crab::Error>> {
    // TODO
    builder.write<vm::op::Code::PRINT>(function_call.token_trace());
    builder.write<vm::op::Code::UNIT>(function_call.token_trace());
    return to_ok(unit{});
  }

  auto Compiler::visit_identifier_binding([[maybe_unused]] const ast::expression::IdentifierBinding &identifier)
      -> Result<std::any, Box<crab::Error>> {

    // TODO: handle no variable found
    const Enviornment::Variable var =
        builder.current_env()->find_variable(identifier.get_identifier()).take_unchecked();

    builder.write_get(builder.get_stack_tracker() - var.stack_offset, identifier.token_trace());

    return to_ok(unit{});
  }

  auto
  Compiler::visit_unary([[maybe_unused]] const ast::expression::Unary &unary) -> Result<std::any, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto Compiler::visit_if([[maybe_unused]] const ast::expression::If &if_expr) -> Result<std::any, Box<crab::Error>> {
    const Label end = builder.make_unlinked_label();
    const Label then{builder.make_unlinked_label()};
    const Label else_then{builder.make_unlinked_label()};

    // condition
    if (auto err = compile(if_expr.get_condition()); err.is_err()) {
      return to_err(err.take_err_unchecked());
    }

    // jump to else_then if false
    builder.write_jmp_if_false(else_then, if_expr.get_else_then().token_trace());

    // then label
    builder.define_label(then);
    if (auto err = compile(if_expr.get_then()); err.is_err()) {
      return to_err(err.take_err_unchecked());
    }
    // jump to end after then
    builder.define_label(else_then);
    builder.write_jmp(end, if_expr.get_else_then().token_trace());

    // else_then label
    if (auto err = compile(if_expr.get_else_then()); err.is_err()) {
      return to_err(err.take_err_unchecked());
    }
    // no jump cause it goes directly to the end
    builder.define_label(end);

    return to_ok(unit{});
  }

  auto Compiler::visit_scope([[maybe_unused]] const ast::expression::ScopeBlock &scope)
      -> Result<std::any, Box<crab::Error>> {
    const usize starting_stack = builder.get_stack_tracker();

    builder.push_env();

    for (const Box<ast::Statement> &statement: scope.get_statements()) {
      if (CompilerResult<unit> err = compile(statement); err.is_err())
        return Box<crab::Error>{err.take_err_unchecked()};
    }

    Result<std::any, Box<crab::Error>> result =
        compile(scope.get_eval()).map([](Value v) { return std::any{v}; }).map_err([](auto err) {
          return Box<crab::Error>{std::move(err)};
        });

    if (result.is_err()) {
      return result.take_err_unchecked();
    }

    builder.pop_env();

    // if (builder.get_stack_tracker() - starting_stack == 1) {
    //   builder.write<vm::op::Code::POP>();
    //   return result;
    // }

    for (usize i = 1; i < builder.get_stack_tracker() - starting_stack; i++) {
      builder.write<vm::op::Code::SWAP>(scope.token_trace());
      builder.write<vm::op::Code::POP>(scope.token_trace());
    }

    return result;
  }

  auto Compiler::visit_while([[maybe_unused]] const ast::expression::While &while_expr)
      -> Result<std::any, Box<crab::Error>> {
    const Label condition = builder.make_unlinked_label();
    const Label end = builder.make_unlinked_label();
    const Label body{builder.make_unlinked_label()};

    // condition
    builder.define_label(condition);
    if (auto err = compile(while_expr.get_condition()); err.is_err()) {
      return to_err(err.take_err_unchecked());
    }

    // jump to else_then if false
    builder.write_jmp_if_false(end, while_expr.token_trace());

    // then label
    builder.define_label(body);
    if (auto err = compile(while_expr.get_body()); err.is_err()) {
      return to_err(err.take_err_unchecked());
    }

    // jump to end after then
    builder.write_jmp(condition, while_expr.token_trace());

    // no jump cause it goes directly to the end
    builder.define_label(end);

    return to_ok(unit{});
  }

  auto Compiler::visit_property_access([[maybe_unused]] const ast::expression::PropertyAccess &property_access)
      -> Result<std::any, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto Compiler::visit_array_index([[maybe_unused]] const ast::expression::ArrayIndex &array_index)
      -> Result<std::any, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto
  Compiler::visit_match([[maybe_unused]] const ast::expression::Match &match) -> Result<std::any, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }
} // namespace goos::codegen
