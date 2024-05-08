//
// Created by bishan_ on 5/8/24.
//

#pragma once
#include "Value.hpp"

namespace goos::json {
  class Object final : public Value {
    Dictionary<WideString, Box<Value>> values;

  public:
    explicit Object();

    auto put(WideString key, Box<Value> val) -> void;

    auto put(WideString key, WideString val) -> void;

    auto erase(const WideString &key) -> bool;

    [[nodiscard]] auto get(const WideString &key) -> Option<RefMut<Value>>;

    [[nodiscard]] auto get(const WideString &key) const -> Option<Ref<Value>>;

    auto write(std::wostream &os) const -> void override;

    auto clone() const -> Box<Value> override;

    auto pairs() const -> const Dictionary<WideString, Box<Value>>& { return values; }
  };
} // goos
