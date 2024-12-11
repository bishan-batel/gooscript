//
// Created by bishan_ on 6/16/24.
//

#pragma once
#include "interfaces/IValue.hpp"
#include "meta/Identifier.hpp"

namespace goos::runtime {
  class LValue final : public IValue {
    RefMut<Any> inner;

    explicit LValue(RefMut<Any> inner);

  public:
    static constexpr meta::VariantType TYPE{meta::VariantType::REFERENCE};

    static auto wrap(Any &value) -> RcMut<LValue>;

    static auto wrap(RefMut<Any> value) -> RcMut<LValue>;

    auto set(Any value) const -> void;

    [[nodiscard]] auto get() const -> Any;

    [[nodiscard]] auto clone() const -> Any override;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    [[nodiscard]] auto base_hash() const -> utils::hash_code override;
    [[nodiscard]] auto to_json() const -> Box<json::Value> override;
  };
}
