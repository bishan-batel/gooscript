//
// Created by bishan_ on 6/27/24.
//

#include "Compiler.hpp"

#include "ast/expression/Binary.hpp"
#include "ast/expression/FunctionCall.hpp"
#include "ast/expression/IdentifierBinding.hpp"
#include "ast/expression/compound/If.hpp"
#include "ast/expression/compound/ScopeBlock.hpp"
#include "ast/expression/compound/While.hpp"
#include "ast/expression/literal/Boolean.hpp"
#include "ast/expression/literal/Decimal.hpp"
#include "ast/expression/literal/Integer.hpp"

namespace goos::codegen {
  Compiler::Compiler(WideString name) : builder{Builder{std::move(name)}} {}

  auto Compiler::to_ok(Value value) -> Result<std::any, Box<crab::Error>> {
    return std::any{value};
  }

  auto Compiler::to_err(Box<err::Error> value) -> Result<std::any, Box<crab::Error>> {
    return Box<crab::Error>{std::move(value)};
  }

  auto Compiler::finalize_chunk() -> vm::Chunk {
    return builder.finalize_chunk();
  }

  auto Compiler::compile(const ast::Statement &statement) -> CompilerResult<unit> {
    Result<unit, Box<crab::Error>> err = statement.accept(*this);

    if (statement.try_as<ast::Expression>().is_some()) {
      builder.write<vm::op::Code::POP>();
    }

    return err.map_err(
      [](Box<crab::Error> error) {
        return Box<err::Error>::wrap_unchecked(&dynamic_cast<err::Error&>(*decltype(error)::unwrap(std::move(error))));
      }
    );
  }

  auto Compiler::compile(const ast::Expression &expr) -> CompilerResult<Value> {
    return
        expr
        .accept_expr(*this)
        .map([](auto c) { return std::any_cast<Value>(c); })
        .map_err(
          [](Box<crab::Error> error) {
            return Box<err::Error>::wrap_unchecked(
              &dynamic_cast<err::Error&>(*decltype(error)::unwrap(std::move(error)))
            );
          }
        );
  }

  auto Compiler::visit_eval([[maybe_unused]] const ast::Eval &eval) -> Result<unit, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto Compiler::visit_return([[maybe_unused]] const ast::Return &ret) -> Result<unit, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto Compiler::visit_variable_declaration(
    [[maybe_unused]]
    const ast::VariableDeclaration &variable_declaration
  ) -> Result<unit, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto Compiler::visit_array(
    [[maybe_unused]] const ast::expression::Array &array
  ) -> Result<std::any, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto Compiler::visit_boolean(
    [[maybe_unused]] const ast::expression::Boolean &boolean
  ) -> Result<std::any, Box<crab::Error>> {
    return to_ok(builder.write_push_constant(boolean.get_state()));
  }

  auto Compiler::visit_decimal(
    [[maybe_unused]] const ast::expression::Decimal &decimal
  ) -> Result<std::any, Box<crab::Error>> {
    return to_ok(builder.write_push_constant(decimal.get_number()));
  }

  auto Compiler::visit_dictionary(
    [[maybe_unused]] const ast::expression::Dictionary &dictionary
  ) -> Result<std::any, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto Compiler::visit_integer(
    [[maybe_unused]] const ast::expression::Integer &integer
  ) -> Result<std::any, Box<crab::Error>> {
    return to_ok(builder.write_push_constant(integer.get_number()));
  }

  auto Compiler::visit_lambda(
    [[maybe_unused]] const ast::expression::Lambda &lambda
  ) -> Result<std::any, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto Compiler::visit_nil([[maybe_unused]] const ast::expression::Nil &nil) -> Result<std::any, Box<crab::Error>> {
    return to_ok(builder.write_push_constant(vm::Nil::val));
  }

  auto Compiler::visit_string_literal(
    [[maybe_unused]] const ast::expression::StringLiteral &str
  ) -> Result<std::any, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto Compiler::visit_unit([[maybe_unused]] const ast::expression::Unit &unit) -> Result<std::any, Box<crab::Error>> {
    return to_ok(builder.write_push_constant(unit::val));
  }

  auto Compiler::visit_binary(
    [[maybe_unused]] const ast::expression::Binary &binary
  ) -> Result<std::any, Box<crab::Error>> {
    debug_assert(lexer::is_binary(binary.get_op()), "Invalid Binary Operator");

    return crab::fallible<Box<err::Error>>(
      [&] { return compile(binary.get_lhs()); },
      [&] { return compile(binary.get_rhs()); }
    ).and_then(
      [&](auto operands) -> CompilerResult<Value> {
        const auto [lhs, rhs] = operands;

        switch (binary.get_op()) {
          // TODO
          // case lexer::Operator::ASSIGN: break;
          // case lexer::Operator::MOD_ASSIGN: break;
          // case lexer::Operator::MUL_ASSIGN: break;
          // case lexer::Operator::DIV_ASSIGN: break;
          // case lexer::Operator::ADD_ASSIGN: break;
          // case lexer::Operator::SUB_ASSIGN: break;
          // case lexer::Operator::BIT_OR_ASSIGN: break;
          // case lexer::Operator::BIT_AND_ASSIGN: break;

          case lexer::Operator::XOR: return builder.write<vm::op::Code::BIT_XOR>();
          case lexer::Operator::BIT_OR: return builder.write<vm::op::Code::BIT_OR>();
          case lexer::Operator::BIT_AND: return builder.write<vm::op::Code::BIT_AND>();
          case lexer::Operator::AND: return builder.write<vm::op::Code::AND>();
          case lexer::Operator::OR: return builder.write<vm::op::Code::OR>();
          case lexer::Operator::NOT: return builder.write<vm::op::Code::NOT>();
          case lexer::Operator::EQUALS: return builder.write<vm::op::Code::EQUALS>();

          case lexer::Operator::NOT_EQUALS: {
            builder.write<vm::op::Code::EQUALS>();
            return builder.write<vm::op::Code::NOT>();
          }

          case lexer::Operator::GREATER: return builder.write<vm::op::Code::GREATER_THAN>();
          case lexer::Operator::LESS: return builder.write<vm::op::Code::LESS_THAN>();
          case lexer::Operator::GREATER_OR_EQUALS: return builder.write<vm::op::Code::GREATER_OR_EQUAL_THAN>();
          case lexer::Operator::LESS_OR_EQUALS: return builder.write<vm::op::Code::LESS_OR_EQUAL_THAN>();
          case lexer::Operator::SHIFT_LEFT: return builder.write<vm::op::Code::SHIFT_LEFT>();
          case lexer::Operator::SHIFT_RIGHT: return builder.write<vm::op::Code::SHIFT_RIGHT>();
          case lexer::Operator::ADD: return builder.write<vm::op::Code::ADD>();
          case lexer::Operator::SUB: return builder.write<vm::op::Code::SUBTRACT>();
          case lexer::Operator::MUL: return builder.write<vm::op::Code::MULIPLY>();
          case lexer::Operator::DIV: return builder.write<vm::op::Code::DIVIDE>();
          case lexer::Operator::MOD: return builder.write<vm::op::Code::MODULO>();

          // case lexer::Operator::IN:
          //   break;

          default:
            return Value{unit{}};
        }
      }
    ).map([](auto v) { return std::any{v}; }).map_err(
      [](auto err) {
        return Box<crab::Error>{std::move(err)};
      }
    );

    // TODO
    return to_ok(unit{});
  }

  auto Compiler::visit_function_call(
    [[maybe_unused]]
    const ast::expression::FunctionCall &function_call
  ) -> Result<std::any, Box<crab::Error>> {
    // TODO
    builder.write<vm::op::Code::PRINT>();
    builder.write<vm::op::Code::UNIT>();
    return to_ok(unit{});
  }

  auto Compiler::visit_identifier_binding(
    [[maybe_unused]]
    const ast::expression::IdentifierBinding &identifier
  ) -> Result<std::any, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto Compiler::visit_unary(
    [[maybe_unused]] const ast::expression::Unary &unary
  ) -> Result<std::any, Box<crab::Error>> {
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
    builder.write_jmp_if_false(else_then);

    // then label
    builder.define_label(then);
    if (auto err = compile(if_expr.get_then()); err.is_err()) {
      return to_err(err.take_err_unchecked());
    }
    // jump to end after then
    builder.write_jmp(end);

    // else_then label
    builder.define_label(else_then);
    if (auto err = compile(if_expr.get_else_then()); err.is_err()) {
      return to_err(err.take_err_unchecked());
    }
    // no jump cause it goes directly to the end
    builder.define_label(end);

    return to_ok(unit{});
  }

  auto Compiler::visit_scope(
    [[maybe_unused]] const ast::expression::ScopeBlock &scope
  ) -> Result<std::any, Box<crab::Error>> {
    for (const Box<ast::Statement> &statement: scope.get_statements()) {
      if (CompilerResult<unit> err = compile(statement); err.is_err())
        return Box<crab::Error>{err.take_err_unchecked()};
    }

    return
        compile(scope.get_eval())
        .map([](Value v) { return std::any{v}; })
        .map_err([](auto err) { return Box<crab::Error>{std::move(err)}; });
  }

  auto Compiler::visit_while(
    [[maybe_unused]] const ast::expression::While &while_expr
  ) -> Result<std::any, Box<crab::Error>> {
    const Label condition = builder.make_unlinked_label();
    const Label end = builder.make_unlinked_label();
    const Label body{builder.make_unlinked_label()};

    // condition
    builder.define_label(condition);
    if (auto err = compile(while_expr.get_condition()); err.is_err()) {
      return to_err(err.take_err_unchecked());
    }

    // jump to else_then if false
    builder.write_jmp_if_false(end);

    // then label
    builder.define_label(body);
    if (auto err = compile(while_expr.get_body()); err.is_err()) {
      return to_err(err.take_err_unchecked());
    }

    // jump to end after then
    builder.write_jmp(condition);

    // no jump cause it goes directly to the end
    builder.define_label(end);

    return to_ok(unit{});
  }

  auto Compiler::visit_property_access(
    [[maybe_unused]]
    const ast::expression::PropertyAccess &property_access
  ) -> Result<std::any, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto Compiler::visit_array_index(
    [[maybe_unused]]
    const ast::expression::ArrayIndex &array_index
  ) -> Result<std::any, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }

  auto Compiler::visit_match(
    [[maybe_unused]] const parser::pass::expr::Match &match
  ) -> Result<std::any, Box<crab::Error>> {
    throw std::logic_error("Not implemented");
  }
}
