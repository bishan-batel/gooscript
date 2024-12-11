//
// Created by bishan_ on 5/15/24.
//

#pragma once
#include "interfaces/IValue.hpp"
#include "runtime/err/RuntimeError.hpp"

namespace goos::runtime {
  class Nil final : public IValue {
    Nil() = default;

    explicit Nil(std::nullptr_t);

  public:
    static constexpr meta::VariantType TYPE{meta::VariantType::NIL};

    static auto value() -> RcMut<Nil>;

    static auto ok() -> Result<std::any, Box<crab::Error>>;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    [[nodiscard]] auto base_hash() const -> usize override;

    [[nodiscard]] auto clone() const -> Any override;

    [[nodiscard]] auto to_json() const -> Box<json::Value> override;
  };
}
