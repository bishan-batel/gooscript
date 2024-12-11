#pragma once

#include <concepts>
#include <preamble.hpp>
#include "json/Value.hpp"

namespace goos::json {

  template<typename T>
  struct Marshal {};

  template<typename T>
  struct UnMarshal {};

  struct IMarshallable {
    [[nodiscard]] virtual auto marshal() const -> Box<json::Value> = 0;
  };

  template<typename T>
  concept marshallable = requires(Marshal<T> m, const T &v) {
    { m(v) } -> std::convertible_to<Box<json::Value>>;
  } or std::derived_from<T, IMarshallable>;

  template<typename T>
  concept unmarshallable = requires(UnMarshal<T> m, const json::Value &v) {
    { m(v) } -> std::convertible_to<Option<T>>;
  };

  template<typename T>
  concept full_marshallable = marshallable<T> and unmarshallable<T>;
  /**
   * @brief Marshals value into JSON
   * @param value Value to marshal
   */
  template<marshallable T>
  auto marshal(const T &value) -> Box<json::Value> {
    if constexpr (std::derived_from<T, IMarshallable>) {
      return value.marshal();
    } else {
      return Marshal<T>{}(value);
    }
  }

  /**
   * @brief Unmarshales a json value
   * @param value Json to unmarshal
   */
  template<unmarshallable T>
  auto unmarshal(const json::Value &value) -> Option<T> {
    return UnMarshal<T>{}(value);
  }
} // namespace goos::json
