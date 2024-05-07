//
// Created by bishan_ on 4/25/24.
//

#pragma once

#include <option.hpp>
#include <preamble.hpp>
#include <box.hpp>

#include <ref.hpp>

namespace goos::ast {
  class Statement {
  public:
    Statement() = default;

    Statement(const Statement &) = delete;

    Statement(Statement &&) = default;

    Statement& operator=(const Statement &) = delete;

    Statement& operator=(Statement &&) = default;

    virtual ~Statement() = default;

    [[nodiscard]] virtual bool operator==(const Statement &statement) const = 0;

    [[nodiscard]] bool operator!=(const Statement &statement) const;

    template<typename T> requires std::is_base_of_v<Statement, T>
    Option<Ref<T>> try_as() const {
      return crab::ref::from_ptr(dynamic_cast<const T*>(this));
    }

    friend std::ostream& operator<<(std::ostream& os, const Statement& statement);

    [[nodiscard]] virtual WideString to_string() const = 0;

    [[nodiscard]] virtual Box<Statement> clone() const = 0;
  };
}
