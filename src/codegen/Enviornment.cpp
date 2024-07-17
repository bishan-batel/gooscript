#include "Enviornment.hpp"
#include <rc.hpp>

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

  auto Enviornment::register_variable(Rc<WideString> name, u64 stack_pos) -> Result<unit, DuplicateName> {
    VariableName key{std::move(name)};

    if (variable_to_stack.contains(key)) {
      return crab::err(DuplicateName{});
    }

    variable_to_stack.emplace(std::move(key), stack_pos);

    return crab::ok(unit{});
  }

  auto Enviornment::find_variable(Rc<WideString> name) const -> Option<usize> {
    VariableName key{name};
    if (variable_to_stack.contains(key)) {
      return variable_to_stack.at(key);
    }

    // attempt to search upwards if there is an enclosing scope
    return Option{enclosing}.flat_map([&](const auto env) { return env->find_variable(name); });
  }
} // namespace goos::codegen
