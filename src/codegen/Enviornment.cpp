#include "Enviornment.hpp"
#include <rc.hpp>
#include "meta/Identifier.hpp"

namespace goos::codegen {
  auto Enviornment::standard_enviornment() -> RcMut<Enviornment> {
    static RcMut<Enviornment> standard_env = [] {
      const auto env = crab::make_rc_mut<Enviornment>();
      return env;
    }();

    return standard_env;
  }

  auto Enviornment::enclose(RcMut<Enviornment> env) -> RcMut<Enviornment> {
    return crab::make_rc_mut<Enviornment>(std::move(env));
  }

  auto Enviornment::register_variable(meta::Identifier key, u64 stack_pos, meta::Mutability mutability)
      -> Result<unit, DuplicateName> {
    if (variable_to_stack.contains(key)) {
      return crab::err(DuplicateName{});
    }

    variable_to_stack.emplace(std::move(key), Variable{stack_pos, mutability});

    return crab::ok(unit{});
  }

  auto Enviornment::find_variable(meta::Identifier key) const -> Option<Variable> {
    if (variable_to_stack.contains(key)) {
      return variable_to_stack.at(key);
    }

    // attempt to search upwards if there is an enclosing scope
    return Option{enclosing}.flat_map([&](const auto env) { return env->find_variable(key); });
  }
} // namespace goos::codegen
