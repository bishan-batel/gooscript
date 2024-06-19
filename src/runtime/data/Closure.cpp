//
// Created by bishan_ on 6/18/24.
//

#include <fmt/xchar.h>
#include <string>
#include "Closure.hpp"
#include "runtime/Intepreter.hpp"

namespace goos::runtime {
  Closure::Closure(RcMut<Environment> captured, Box<ast::expression::Lambda> lambda)
    : lambda{std::move(lambda)}, captured{std::move(captured)} {}

  auto Closure::call(Intepreter &runtime, const Vec<Any> &values) const -> Result<Any> {
    using namespace meta;

    RcMut<Environment> prev_env = runtime.get_env();

    runtime.set_env(captured);

    runtime.push_env(); {
      namespace rv = std::ranges::views;

      const Vec<Identifier> &params = lambda->get_params();
      Environment &env = runtime.env();

      const usize required_args = get_arity();
      const usize given_args = std::min(values.size(), required_args);

      for (const usize i: crab::range(given_args)) {
        env.push_variable(params[i], Mutability::IMMUTABLE, values[i]);
      }

      if (given_args < required_args) {
        for (const usize i: crab::range(given_args, required_args)) {
          env.push_variable(params[i], Mutability::IMMUTABLE, Unit::value());
        }
      }
    }

    Result<Any> value = runtime.evaluate(lambda->get_body());

    // pop arguments
    runtime.pop_env();

    // return to previous enviornment
    runtime.set_env(std::move(prev_env));

    if (value.is_err()) return value.take_err_unchecked();

    if (auto returned = runtime.consume_halt_flag(ControlFlowFlag::RETURN)) {
      return returned.take_unchecked();
    }
    return value;
  }

  auto Closure::get_arity_type() const -> ArityType {
    return ArityType::Finite;
  }

  auto Closure::get_arity() const -> usize {
    return this->lambda->get_params().size();
  }

  auto Closure::clone() const -> Any {
    auto *cloned = dynamic_cast<ast::expression::Lambda*>(Box<
      ast::Expression>::unwrap(lambda->clone_expr()));

    return crab::make_rc_mut<Closure>(
      captured,
      Box<ast::expression::Lambda>::wrap_unchecked(cloned)
    );
  }

  auto Closure::to_string() const -> WideString {
    return fmt::format(L"func[{}]", lambda->to_string());
  }

  auto Closure::get_type() const -> meta::VariantType {
    return meta::VariantType::FUNCTION;
  }

  auto Closure::base_hash() const -> utils::hash_code {
    return utils::hash(this);
  }
}
