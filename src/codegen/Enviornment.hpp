#pragma once

#include "Value.hpp"
#include "utils/hash.hpp"

namespace goos::codegen {
  struct VariableName final {
    Rc<WideString> value;

    auto operator==(const VariableName &l) const -> bool { return *value == *l.value; }
  };
} // namespace goos::codegen

template<>
struct std::hash<goos::codegen::VariableName> {
  [[nodiscard]] auto operator()(const goos::codegen::VariableName &val) const noexcept -> usize {
    return goos::utils::hash(val.value.get());
  }
};

static_assert(goos::utils::hashable<goos::codegen::VariableName>, "Failed hashing impl");

namespace goos::codegen {
  class Enviornment final {
    Option<RcMut<Enviornment>> enclosing{crab::none};
    usize depth{0};

    Dictionary<VariableName, usize> variable_to_stack{};

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

    [[nodiscard]] auto register_variable(Rc<WideString> name, u64 stack_pos) -> Result<unit, DuplicateName>;

    [[nodiscard]] auto find_variable(Rc<WideString> name) const -> Option<usize>;

    [[nodiscard]] auto is_top_level() const -> bool { return enclosing.is_some(); }

    [[nodiscard]] auto get_enclosing() const -> Option<RcMut<Enviornment>> { return enclosing; }
  };
} // namespace goos::codegen
