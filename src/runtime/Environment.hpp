//
// Created by bishan_ on 5/15/24.
//

#pragma once

#include "preamble.hpp"
#include "data/TypeConversion.hpp"
#include "meta/Identifier.hpp"
#include "meta/Mutability.hpp"
#include "meta/VariantType.hpp"

namespace goos::runtime {
  class Intepreter;
  class ExternFunction;

  class Variable final {
    meta::Mutability mutability;
    RcMut<IValue> value;
    meta::VariantType type;

  public:
    Variable(meta::Mutability mutability, RcMut<IValue> value);

    Variable(const Variable &) = delete;

    auto operator=(const Variable &) -> Variable& = delete;

    ~Variable() = default;

    [[nodiscard]] auto get_mutability() const -> meta::Mutability;

    [[nodiscard]] auto get_value() const -> RcMut<IValue>;

    [[nodiscard]] auto get_type() const -> meta::VariantType;

    auto set_value(RcMut<IValue> value) -> void;
  };

  class Environment final {
    Dictionary<meta::Identifier, RcMut<Variable>> bindings{};
    Option<RcMut<Environment>> parent;
    usize depth{0};
    RefMut<Intepreter> intepreter;

  public:
    auto define_builtin(WideString name, const RcMut<ExternFunction> &function) -> void;

    template<typename F>
    auto define_builtin(WideString name, F function) -> void;

    static auto get_standard_environment(Intepreter &intepreter) -> RcMut<Environment>;

    explicit Environment(Intepreter &intepreter, RcMut<Environment> parent);

    explicit Environment(Intepreter &intepreter, Option<RcMut<Environment>> parent = crab::none);

    static auto enclose(RcMut<Environment> enclosing) -> RcMut<Environment>;

    template<typename IntoIdentifier, typename ValueTy> requires
      std::is_constructible_v<meta::Identifier, IntoIdentifier> and
      type::is_value<ValueTy>
    auto define_constant(IntoIdentifier identifier, RcMut<ValueTy> value) -> Result<RcMut<Variable>>;

    auto push_variable(
      meta::Identifier identifier,
      meta::Mutability mutability,
      RcMut<IValue> value
    ) -> Result<RcMut<Variable>>;

    auto set_value(const meta::Identifier &identifier, RcMut<IValue> value) const -> Result<RcMut<Variable>>;

    [[nodiscard]] auto get_variable(const meta::Identifier &identifier) const -> Result<RcMut<Variable>>;

    [[nodiscard]] auto get_depth() const -> usize;

    auto get_previous() const -> Option<RcMut<Environment>>;

    auto runtime() const -> Intepreter&;
  };

  namespace lambda {
    template<typename F>
    static auto from(F function) -> RcMut<ExternFunction>;
  };

  template<typename F>
  auto Environment::define_builtin(WideString name, F function) -> void {
    define_builtin(std::move(name), lambda::from(function));
  }

  template<typename IntoIdentifier, typename ValueTy> requires
    std::is_constructible_v<meta::Identifier, IntoIdentifier> and
    type::is_value<ValueTy>
  auto Environment::define_constant(IntoIdentifier identifier, RcMut<ValueTy> value) -> Result<RcMut<Variable>> {
    return push_variable(meta::Identifier{identifier}, meta::Mutability::IMMUTABLE, Any{value});
  }

  // template<typename T, typename V> requires std::is_constructible_v<meta::Identifier, T>
  // auto Environment::define_constant(T identifier, V value) -> Result<RcMut<Variable>> {
  //   return push_variable(
  //     meta::Identifier{identifier},
  //     meta::Mutability::IMMUTABLE,
  //     type::to_goos_any(std::move(value))
  //   );
  // }
}
