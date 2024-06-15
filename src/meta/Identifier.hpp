//
// Created by bishan_ on 6/13/24.
//

#pragma once
#include <string_view>

#include "preamble.hpp"
#include "ast/expression/literal/Array.hpp"
#include "utils/str.hpp"

namespace goos::meta {
  class Identifier {
  public:
    using Hash = usize;

  private:
    Rc<WideString> name;
    Hash hash = 0;

  public:
    explicit Identifier(WideString name);

    template<typename T> requires std::is_convertible_v<T, WideString>
    explicit Identifier(T name);

    template<typename T> requires std::is_convertible_v<T, StringView>
    explicit Identifier(T name);

    // ReSharper disable once CppNonExplicitConversionOperator
    operator // NOLINT(*-explicit-constructor)
    const WideString&() const;

    [[nodiscard]] auto get_string() const -> const WideString&;

    [[nodiscard]] auto get_hash() const -> Hash;

    auto operator==(const Identifier &identifier) const -> bool;

  private:
    auto compute_hash() -> void;
  };

  template<typename T> requires std::is_convertible_v<T, WideString>
  Identifier::Identifier(T name) : Identifier{static_cast<WideString>(std::move(name))} {}

  template<typename T> requires std::is_convertible_v<T, StringView>
  Identifier::Identifier(T name) : Identifier{str::convert(static_cast<StringView>(name))} {}
}

template<>
struct std::hash<goos::meta::Identifier> {
  auto operator ()(const goos::meta::Identifier &ident) const noexcept -> usize;
};
