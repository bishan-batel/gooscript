//
// Created by bishan_ on 5/8/24.
//

#pragma once
#include "Value.hpp"
#include <concepts>

namespace goos::json {
  class Object final : public Value {
    Dictionary<WideString, Box<Value>> values;

  public:

    explicit Object();

    auto put(WideString key, Box<Value> val) -> void;

    template<std::derived_from<Value> T>
    auto put(WideString key, Box<T> val) -> void {
      put(key, std::move(val));
    }

    template<typename T>
    auto put(WideString key, T val) -> void;

    auto erase(const WideString& key) -> bool;

    [[nodiscard]] auto get(const WideString& key) -> Option<RefMut<Value>>;

    [[nodiscard]] auto get(const WideString& key) const -> Option<Ref<Value>>;

    [[nodiscard]] auto operator[](const WideString& key) -> Value&;

    template<std::derived_from<Value> T>
    [[nodiscard]] auto get_as(const WideString& key) const -> Option<Ref<T>> {
      return get(key) //
        .flat_map([&](const Value& val) { return val.try_as<T>(); });
    }

    template<std::derived_from<Value> T>
    [[nodiscard]] auto get_as(const WideString& key) -> Option<RefMut<T>> {
      return get(key).flat_map([&](RefMut<Value> val) {
        return val->try_as<T>();
      });
    }

    auto write(std::wostream& os) const -> void override;

    auto clone() const -> Box<Value> override;

    auto pairs() const -> const Dictionary<WideString, Box<Value>>& {
      return values;
    }
  };

} // namespace goos::json

#include "json/utils.hpp"

template<typename T>
auto goos::json::Object::put(WideString key, T val) -> void {
  return put(std::move(key), json::to_json(val));
}
