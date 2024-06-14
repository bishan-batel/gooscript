//
// Created by bishan_ on 5/15/24.
//

#pragma once

#include "preamble.hpp"
#include "meta/Identifier.hpp"
#include "meta/Mutability.hpp"
#include "meta/VariantType.hpp"

namespace goos::runtime {
  class Variable final {
    meta::Mutability mutability;
    RcMut<Value> value;
    meta::VariantType type;

  public:
    Variable(meta::Mutability mutability, RcMut<Value> value);

    Variable(const Variable &) = delete;

    auto operator=(const Variable &) -> Variable& = delete;

    ~Variable() = default;

    [[nodiscard]] auto get_mutability() const -> meta::Mutability;

    [[nodiscard]] auto get_value() const -> RcMut<Value>;

    [[nodiscard]] auto get_type() const -> meta::VariantType;

    auto set_value(RcMut<Value> value) -> void;
  };

  class Environment final {
    Dictionary<meta::Identifier, RcMut<Variable>> bindings{};
    Option<RcMut<Environment>> parent;
    usize depth{0};

  public:
    explicit Environment(RcMut<Environment> parent);

    explicit Environment(Option<RcMut<Environment>> parent = crab::none);

    auto push_variable(meta::Identifier identifier, meta::Mutability mutability, RcMut<Value> value) -> void;

    auto set_value(const meta::Identifier &identifier, RcMut<Value> value) -> void;

    [[nodiscard]] auto get_variable(const meta::Identifier &identifier) const -> RcMut<Variable>;

    [[nodiscard]] auto try_get_variable(const meta::Identifier &identifier) const -> Option<RcMut<Variable>>;

    [[nodiscard]] auto get_depth() const -> usize;

    auto get_previous() const -> Option<RcMut<Environment>>;
  };
}
