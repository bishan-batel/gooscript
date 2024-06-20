//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include "interfaces/IValue.hpp"
#include "meta/Identifier.hpp"

namespace goos::runtime {
  class GString final : public IValue {
    Rc<WideString> text;
    // mutable usize cached_hash;
    mutable usize cached_hash;
    // mutable bool is_hash_dirty;

  public:
    using Contained = WideString;

    static constexpr meta::VariantType TYPE{meta::VariantType::STRING};

    explicit GString(WideString string = L"");

    explicit GString(WideStringView string);

    explicit GString(const widechar* string);

    explicit GString(const meta::Identifier &identifier);

    explicit GString(Rc<WideString> string);

    [[nodiscard]] auto get() const -> Rc<WideString>;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    [[nodiscard]] auto base_hash() const -> usize override;

    [[nodiscard]] auto clone() const -> Any override;
  };
}
