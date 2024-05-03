//
// Created by bishan_ on 5/2/24.
//

#include "BuiltinLambda.hpp"

#include <fmt/compile.h>

namespace goos::ast::expression {
  BuiltinLambda::BuiltinLambda(Callback callback) : builtin_callback{std::move(callback)} {}

  String BuiltinLambda::to_string() const {
    return "@builtin fn";
  }

  Option<meta::VariantType> BuiltinLambda::variant_type() const {
    return crab::none;
  }
}
