#pragma once

#include "Value.hpp"
#include "meta/Identifier.hpp"
#include "meta/Mutability.hpp"
#include "runtime/Environment.hpp"
#include "utils/hash.hpp"

namespace goos::codegen {
  class Enviornment final {
  public:
    struct Variable {
      u64 stack_offset;
      meta::Mutability mutability;
    };

  private:
    Option<RcMut<Enviornment>> enclosing{crab::none};
    usize depth{0};

    Dictionary<meta::Identifier, Variable> variable_to_stack{};

  public:
    Enviornment() = default;

    Enviornment(RcMut<Enviornment> env) : enclosing{env}, depth{env->depth + 1} {}

    [[nodiscard]] static auto standard_enviornment() -> RcMut<Enviornment>;

    /**
     * @brief Creates a new enviornment that is s
     *
     */
    [[nodiscard]] static auto enclose(RcMut<Enviornment> env) -> RcMut<Enviornment>;

    struct DuplicateName final : crab::Error {
      [[nodiscard]] auto what() const -> String override { return "Duplicate variable name"; }
    };

    [[nodiscard]] auto register_variable(meta::Identifier name, u64 stack_pos, meta::Mutability mutability) -> Result<unit, DuplicateName>;

    [[nodiscard]] auto find_variable(meta::Identifier name) const -> Option<Variable>;

    [[nodiscard]] auto is_top_level() const -> bool { return enclosing.is_some(); }

    [[nodiscard]] auto get_enclosing() const -> Option<RcMut<Enviornment>> { return enclosing; }
  };
} // namespace goos::codegen
