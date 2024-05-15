//
// Created by bishan_ on 4/25/24.
//

#pragma once

#include <option.hpp>
#include <preamble.hpp>
#include <box.hpp>
#include <memory>

#include <ref.hpp>

#include "json/Value.hpp"

namespace goos {
  namespace runtime {
    class Value;
  }

  namespace ast {
    namespace expression {
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
        virtual auto visit_eval(const Eval &eval) -> void = 0;

        virtual auto visit_return(const Return &ret) -> void = 0;

        virtual auto visit_variable_declaration(const VariableDeclaration &variable_declaration) -> void = 0;
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

      virtual auto accept(IVisitor &visitor) const -> void = 0;
    };

    template<typename T> requires std::is_base_of_v<Statement, T>
    auto Statement::try_as() const -> Option<Ref<T>> {
      return crab::ref::from_ptr(dynamic_cast<const T*>(this));
    }
  }
}
