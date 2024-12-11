//
// Created by bishan_ on 6/16/24.
//

#pragma once
#include "../../meta/VariantType.hpp"
#include "interfaces/IValue.hpp"
#include "LValue.hpp"
#include "interfaces/IIndexible.hpp"

namespace goos::runtime {
  class Array final : public IIndexible {
    Vec<Any> values{};

  public:
    explicit Array(Vec<Any> values = {});

    auto push(Any value) -> void;

    auto at(usize i) -> RcMut<LValue>;

    auto operator[](usize i) -> RcMut<LValue>;

    [[nodiscard]] auto clone() const -> Any override;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    [[nodiscard]] auto base_hash() const -> utils::hash_code override;

    [[nodiscard]] auto get() const -> const Vec<Any>&;

    [[nodiscard]] auto length() const -> usize;

    [[nodiscard]] auto index(Any index) -> Result<Any, Box<err::Error>> override;

    [[nodiscard]] auto to_json() const -> Box<json::Value> override;
    constexpr static auto TYPE = meta::VariantType::ARRAY;
  };
} // goos
