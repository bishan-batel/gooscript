//
// Created by bishan_ on 6/13/24.
//

#pragma once
#include <string_view>

#include "preamble.hpp"
#include "ast/expression/literal/Array.hpp"
#include "utils/str.hpp"

namespace goos::runtime {
  class GString;
}

namespace goos::meta {
  class Identifier {
  public:
    using Hash = usize;

  private:
    Rc<WideString> name;
    Hash hash{0};

    explicit Identifier(Rc<WideString> name);

  public:
    static auto wrap(Rc<WideString> name) -> Identifier;

    static auto from(WideString name) -> Identifier;

    static auto from(StringView name) -> Identifier;

    static auto from(const runtime::GString &name) -> Identifier;

    // explicit Identifier(WideString name);
    //
    // explicit Identifier(const runtime::GString &name);
    //
    // explicit Identifier(WideStringView name);
    //
    // explicit Identifier(StringView name);

    // ReSharper disable once CppNonExplicitConversionOperator
    operator // NOLINT(*-explicit-constructor)
    const WideString&() const;

    [[nodiscard]] auto get_string() const -> const WideString&;

    [[nodiscard]] auto get_string_ref_counted() const -> Rc<WideString>;

    [[nodiscard]] auto get_hash() const -> Hash;

    auto operator==(const Identifier &identifier) const -> bool;
  };
}

template<>
struct std::hash<goos::meta::Identifier> {
  auto operator ()(const goos::meta::Identifier &ident) const noexcept -> usize;
};
