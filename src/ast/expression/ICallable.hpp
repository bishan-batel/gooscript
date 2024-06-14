//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include <option.hpp>

#include "runtime/data/Value.hpp"

namespace goos::ast::expression {
  enum ArityType {
    Varargs,
    Finite
  };

  class ICallable {
  public:
    virtual ~ICallable() = default;

    [[nodiscard]] virtual auto call(Vec<runtime::Result<runtime::Any>> values) const -> runtime::Result<runtime::Any> = 0;

    [[nodiscard]] virtual auto get_arity_type() const -> ArityType = 0;

    [[nodiscard]] virtual auto get_arity() const -> usize = 0;
  };
}
