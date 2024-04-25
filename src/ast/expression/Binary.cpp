//
// Created by bishan_ on 4/25/24.
//

#include "Binary.hpp"

#include "build/Release/_deps/fmt-src/include/fmt/compile.h"

namespace goos::ast::expression {
  Binary::Binary(Box<Expression> lhs, const lexer::Operator op, Box<Expression> rhs)
    : lhs(std::move(lhs)), rhs(std::move(rhs)), op(op) {}

  const Expression &Binary::get_lhs() const { return lhs; }

  const Expression &Binary::get_rhs() const { return rhs; }

  String Binary::to_string() const {
    return fmt::format(
      "({}) {} ({})",
      lhs->to_string(),
      lexer::OPERATOR_TO_STR_MAP.at(op),
      rhs->to_string()
    );
  }

  Option<meta::VariantType> Binary::variant_type() {
    auto left_variant = lhs->variant_type();
    auto right_variant = rhs->variant_type();

    if (left_variant.is_none() || right_variant.is_none()) {
      return crab::none;
    }

    return left_variant.get_unchecked() == right_variant.get_unchecked()
             ? left_variant
             : crab::none;
  }
}
