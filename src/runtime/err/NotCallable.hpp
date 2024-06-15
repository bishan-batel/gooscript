//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include "RuntimeError.hpp"
#include "ast/Statement.hpp"

namespace goos::runtime::err {
  class NotCallable final : public Error {
    Any value;

  public:
    explicit NotCallable(Any value);

    [[nodiscard]] auto get_value() const -> Any;

    [[nodiscard]] String what() const override;
  };
}
