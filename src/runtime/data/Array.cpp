//
// Created by bishan_ on 6/16/24.
//

#include "Array.hpp"

#include <sstream>

#include "LValue.hpp"
#include "TypeConversion.hpp"

namespace goos {
  runtime::Array::Array(Vec<Any> values): values{std::move(values)} {}

  auto runtime::Array::push(Any value) -> void {
    values.push_back(std::move(value));
  }

  auto runtime::Array::at(const usize i) -> RcMut<LValue> {
    // TODO error hadnling + dependency handling bc fuckin RefMut<> moment

    return LValue::wrap(values[i]);
  }

  auto runtime::Array::operator[](const usize i) -> RcMut<LValue> {
    return at(i);
  }

  auto runtime::Array::clone() const -> Any {
    auto arr = crab::make_rc_mut<Array>();

    for (const auto &ele: values) {
      arr->push(ele->clone());
    }

    return arr;
  }

  auto runtime::Array::to_string() const -> WideString {
    WideStringStream stream;
    stream << '[';
    for (const auto &ele: values) {
      stream << ele->to_string();
      stream << ", ";
    }
    stream << ']';
    return stream.str();
  }

  auto runtime::Array::get_type() const -> meta::VariantType {
    return TYPE;
  }

  auto runtime::Array::base_hash() const -> utils::hash_code {
    utils::hash_code code{values.size()};
    for (const auto &ele: values) {
      code = utils::hash_together(ele->hash());
    }
    return code;
  }

  auto runtime::Array::get() const -> const Vec<Any>& {
    return values;
  }

  auto runtime::Array::length() const -> usize {
    return values.size();
  }

  auto runtime::Array::index(const Any index) -> Result<Any> {
    auto integer = type::coerce<Integer>(index);
    if (integer.is_err()) return integer.take_err_unchecked();

    // TODO out of bounds index
    const auto i = static_cast<usize>(integer.take_unchecked()->get());
    return ok(LValue::wrap(values.at(i)));
  }
} // goos
