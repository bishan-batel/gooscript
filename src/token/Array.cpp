//
// Created by bishan_ on 6/16/24.
//

#include "Array.hpp"

#include <sstream>

namespace goos {
  auto runtime::Array::push(Any value) -> void {
    values.push_back(std::move(value));
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

  auto runtime::Array::get_values() const -> Vec<Any> {
    return values;
  }

  auto runtime::Array::length() const -> usize {
    return values.size();
  }
} // goos
