//
// Created by bishan_ on 4/25/24.
//

#pragma once

#include <option.hpp>
#include <preamble.hpp>
#include <box.hpp>
#include <rc.hpp>

#include <ref.hpp>

#include "json/Value.hpp"
#include <result.hpp>

#include "runtime/data/IValue.hpp"
#include "runtime/err/RuntimeError.hpp"

namespace goos::ast::expression {
  class ArrayIndex;
}

namespace goos::ast::expression {
  class PropertyAccess;
}

namespace goos {
  namespace runtime {
    struct IValue;
  }

  namespace ast {
    namespace expression {
      class While;
      class ScopeBlock;
      class If;
      class Unary;
      class IdentifierBinding;
      class FunctionCall;
      class Binary;
      class Unit;
      class StringLiteral;
      class Nil;
      class Lambda;
      class Integer;
      class Dictionary;
      class Decimal;
      class Boolean;
      class Array;
    }

    class VariableDeclaration;
    class Return;
    class Eval;

    class Statement {
    public:
      struct IVisitor {
        virtual ~IVisitor() = default;

        // Statements
        virtual auto visit_eval(const Eval &eval) -> runtime::VoidResult = 0;

        virtual auto visit_return(const Return &ret) -> runtime::VoidResult = 0;

        virtual auto visit_variable_declaration(
          const VariableDeclaration &variable_declaration
        ) -> runtime::VoidResult = 0;

        // Expressions
        virtual auto visit_array(const expression::Array &array) -> runtime::Result<runtime::Any> = 0;

        virtual auto visit_boolean(const expression::Boolean &boolean) -> runtime::Result<runtime::Any> = 0;

        virtual auto visit_decimal(const expression::Decimal &decimal) -> runtime::Result<runtime::Any> = 0;

        virtual auto visit_dictionary(const expression::Dictionary &dictionary) -> runtime::Result<runtime::Any> = 0;

        virtual auto visit_integer(const expression::Integer &integer) -> runtime::Result<runtime::Any> = 0;

        virtual auto visit_lambda(const expression::Lambda &lambda) -> runtime::Result<runtime::Any> = 0;

        virtual auto visit_nil(const expression::Nil &nil) -> runtime::Result<runtime::Any> = 0;

        virtual auto visit_string_literal(const expression::StringLiteral &str) -> runtime::Result<runtime::Any> = 0;

        virtual auto visit_unit(const expression::Unit &unit) -> runtime::Result<runtime::Any> = 0;

        virtual auto visit_binary(const expression::Binary &binary) -> runtime::Result<runtime::Any> = 0;

        virtual auto visit_function_call(
          const expression::FunctionCall &function_call
        ) -> runtime::Result<runtime::Any> = 0;

        virtual auto visit_identifier_binding(
          const expression::IdentifierBinding &identifier
        ) -> runtime::Result<runtime::Any> = 0;

        virtual auto visit_unary(const expression::Unary &unary) -> runtime::Result<runtime::Any> = 0;

        virtual auto visit_if(const expression::If &if_expr) -> runtime::Result<runtime::Any> = 0;

        virtual auto visit_scope(const expression::ScopeBlock &scope) -> runtime::Result<runtime::Any> = 0;

        virtual auto visit_while(const expression::While &while_expr) -> runtime::Result<runtime::Any> = 0;

        virtual auto visit_property_access(
          const expression::PropertyAccess &property_access
        ) -> runtime::Result<runtime::Any> = 0;

        virtual auto visit_array_index(
          const expression::ArrayIndex &array_index
        ) -> runtime::Result<runtime::Any> = 0;
      };

      Statement() = default;

      Statement(const Statement &) = delete;

      Statement(Statement &&) = default;

      auto operator=(const Statement &) -> Statement& = delete;

      auto operator=(Statement &&) -> Statement& = default;

      virtual ~Statement() = default;

      [[nodiscard]] virtual auto operator==(const Statement &statement) const -> bool = 0;

      [[nodiscard]] auto operator!=(const Statement &statement) const -> bool;

      template<typename T> requires std::is_base_of_v<Statement, T>
      auto try_as() const -> Option<Ref<T>>;

      friend auto operator<<(std::ostream &os, const Statement &statement) -> std::ostream&;

      [[nodiscard]] virtual auto to_string() const -> WideString = 0;

      [[nodiscard]] virtual auto json() const -> Box<json::Value> = 0;

      [[nodiscard]] virtual auto clone() const -> Box<Statement> = 0;

      virtual auto accept(IVisitor &visitor) const -> runtime::VoidResult = 0;
    };

    template<typename T> requires std::is_base_of_v<Statement, T>
    auto Statement::try_as() const -> Option<Ref<T>> {
      return crab::ref::from_ptr(dynamic_cast<const T*>(this));
    }
  }
}
