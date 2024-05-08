//
// Created by bishan_ on 5/8/24.
//

#pragma once
#include <option.hpp>
#include <ostream>
#include <ref.hpp>
#include <box.hpp>

namespace goos::json {
  auto escape(std::wostream &os, WideStringView str) -> void;

  class Value {
  public:
    Value() = default;

    Value(const Value &) = delete;

    Value(Value &&) = default;

    virtual ~Value() = default;

    auto operator=(const Value &) -> void = delete;

    auto operator=(Value &&) -> Value& = default;

    template<typename T> requires std::is_base_of_v<Value, T>
    [[nodiscard]] auto try_as() const -> Option<Ref<T>>;

    template<typename T> requires std::is_base_of_v<Value, T>
    [[nodiscard]] auto try_as() -> Option<RefMut<T>>;

    virtual auto write(std::wostream &os) const -> void = 0;

    friend auto operator <<(std::wostream &os, const Value &value) -> std::wostream&;

    virtual auto clone() const -> Box<Value> = 0;
  };

  template<typename T> requires std::is_base_of_v<Value, T>
  auto Value::try_as() const -> Option<Ref<T>> {
    return crab::ref::cast<T>(*this);
  }

  template<typename T> requires std::is_base_of_v<Value, T>
  auto Value::try_as() -> Option<RefMut<T>> {
    return crab::ref::cast<T>(*this);
  }
}
