//
// Created by bishan_ on 5/8/24.
//

#include "Object.hpp"
#include <box.hpp>
#include <option.hpp>

#include "Text.hpp"
#include "Value.hpp"
#include "json/Number.hpp"

namespace goos::json {
  Object::Object() = default;

  auto Object::put(WideString key, Box<Value> val) -> void {
    erase(key);
    values.emplace(std::move(key), std::move(val));
  }

  // auto Object::put(WideString key, WideString val) -> void {
  //   values.emplace(key, crab::make_box<Text>(std::move(val)));
  // }

  auto Object::erase(const WideString &key) -> bool {
    if (values.contains(key)) {
      values.erase(key);
    }
    return false;
  }

  auto Object::get(const WideString &key) -> Option<RefMut<Value>> {
    if (values.contains(key)) {
      return crab::some(RefMut{*values.at(key)});
    }

    return crab::none;
  }

  auto Object::get(const WideString &key) const -> Option<Ref<Value>> {
    if (values.contains(key)) {
      return crab::some(Ref{*values.at(key)});
    }

    return crab::none;
  }

  auto Object::write(std::wostream &os) const -> void {
    os << '{';
    usize count = 0;

    for (const auto &[key, v]: pairs()) {
      os << '"';
      escape(os, key);
      os << "\":";
      v->write(os);
      if (++count < values.size()) os << ',';
    }
    os << '}';
  }

  auto Object::clone() const -> Box<Value> {
    auto clone{crab::make_box<Object>()};

    for (const auto &[k, v]: pairs()) {
      clone->put(k, v->clone());
    }

    return clone;
  }

  auto Object::operator[](const WideString &key) -> Value & {
    if (Option<RefMut<Value>> ref = get(key)) {
      return ref.take_unchecked();
    }

    put(key, crab::make_box<Number>(0));

    return get(key).take_unchecked();
  }
} // namespace goos::json
