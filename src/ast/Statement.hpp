//
// Created by bishan_ on 4/25/24.
//

#pragma once

#include <box.hpp>
#include <option.hpp>
#include <preamble.hpp>
#include <rc.hpp>

#include <ref.hpp>

#include <any>
#include <result.hpp>
#include "json/Value.hpp"
#include "TokenTrace.hpp"

namespace goos {
  namespace ast {
    class TokenTrace;

    namespace expression {
      class ArrayIndex;
      class PropertyAccess;
      class While;
      class ScopeBlock;
      class If;
      class Unary;
      class Match;
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
    } // namespace expression

    class VariableDeclaration;
    class Return;
    class Eval;

    class Statement {
    public:
      struct IVisitor {
        virtual ~IVisitor() = default;
        // Statements
        virtual auto visit_eval(const Eval &eval) -> Result<unit, Box<crab::Error>> = 0;

        virtual auto visit_return(const Return &ret) -> Result<unit, Box<crab::Error>> = 0;

        virtual auto visit_variable_declaration(const VariableDeclaration &variable_declaration)
            -> Result<unit, Box<crab::Error>> = 0;

        // Expressions
        virtual auto visit_array(const expression::Array &array) -> Result<std::any, Box<crab::Error>> = 0;

        virtual auto visit_boolean(const expression::Boolean &boolean) -> Result<std::any, Box<crab::Error>> = 0;

        virtual auto visit_decimal(const expression::Decimal &decimal) -> Result<std::any, Box<crab::Error>> = 0;

        virtual auto
        visit_dictionary(const expression::Dictionary &dictionary) -> Result<std::any, Box<crab::Error>> = 0;

        virtual auto visit_integer(const expression::Integer &integer) -> Result<std::any, Box<crab::Error>> = 0;

        virtual auto visit_lambda(const expression::Lambda &lambda) -> Result<std::any, Box<crab::Error>> = 0;

        virtual auto visit_nil(const expression::Nil &nil) -> Result<std::any, Box<crab::Error>> = 0;

        virtual auto
        visit_string_literal(const expression::StringLiteral &str) -> Result<std::any, Box<crab::Error>> = 0;

        virtual auto visit_unit(const expression::Unit &unit) -> Result<std::any, Box<crab::Error>> = 0;

        virtual auto visit_binary(const expression::Binary &binary) -> Result<std::any, Box<crab::Error>> = 0;

        virtual auto
        visit_function_call(const expression::FunctionCall &function_call) -> Result<std::any, Box<crab::Error>> = 0;

        virtual auto visit_identifier_binding(const expression::IdentifierBinding &identifier)
            -> Result<std::any, Box<crab::Error>> = 0;

        virtual auto visit_unary(const expression::Unary &unary) -> Result<std::any, Box<crab::Error>> = 0;

        virtual auto visit_if(const expression::If &if_expr) -> Result<std::any, Box<crab::Error>> = 0;

        virtual auto visit_scope(const expression::ScopeBlock &scope) -> Result<std::any, Box<crab::Error>> = 0;

        virtual auto visit_while(const expression::While &while_expr) -> Result<std::any, Box<crab::Error>> = 0;

        virtual auto visit_property_access(const expression::PropertyAccess &property_access)
            -> Result<std::any, Box<crab::Error>> = 0;

        virtual auto
        visit_array_index(const expression::ArrayIndex &array_index) -> Result<std::any, Box<crab::Error>> = 0;

        virtual auto visit_match(const expression::Match &match) -> Result<std::any, Box<crab::Error>> = 0;
      };

      Statement() = default;

      Statement(const Statement &) = delete;

      Statement(Statement &&) = default;

      auto operator=(const Statement &) -> Statement & = delete;

      auto operator=(Statement &&) -> Statement & = default;

      virtual ~Statement() = default;

      [[nodiscard]] virtual auto operator==(const Statement &statement) const -> bool = 0;

      [[nodiscard]] auto operator!=(const Statement &statement) const -> bool;

      template<typename T>
        requires std::is_base_of_v<Statement, T>
      auto try_as() const -> Option<Ref<T>>;

      friend auto operator<<(std::ostream &os, const Statement &statement) -> std::ostream &;

      [[nodiscard]] virtual auto to_string() const -> WideString = 0;

      [[nodiscard]] virtual auto json() const -> Box<json::Value> = 0;

      [[nodiscard]] virtual auto clone() const -> Box<Statement> = 0;

      [[nodiscard]] virtual auto token_trace() const -> TokenTrace = 0;

      virtual auto accept(IVisitor &visitor) const -> Result<unit, Box<crab::Error>> = 0;
    };

    template<typename T>
      requires std::is_base_of_v<Statement, T>
    auto Statement::try_as() const -> Option<Ref<T>> {
      return crab::ref::from_ptr(dynamic_cast<const T *>(this));
    }
  } // namespace ast
} // namespace goos
