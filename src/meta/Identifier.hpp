//
// Created by bishan_ on 6/13/24.
//

#pragma once
#include <string_view>

#include "preamble.hpp"
#include "ast/expression/literal/Array.hpp"

namespace goos::meta {
  class Identifier {
  public:
    using Hash = usize;

  private:
    Rc<WideString> name;
    Hash hash = 0;

  public:
    explicit Identifier(WideString name);

    // ReSharper disable once CppNonExplicitConversionOperator
    operator // NOLINT(*-explicit-constructor)
    const WideString&() const;

    [[nodiscard]] auto get_string() const -> const WideString&;

    [[nodiscard]] auto get_hash() const -> Hash;

    auto operator==(const Identifier &identifier) const -> bool;

  private:
    auto compute_hash() -> void;
  };
}

template<>
struct std::hash<goos::meta::Identifier> {
  auto operator ()(const goos::meta::Identifier &ident) const noexcept -> usize;
};
