//
// Created by bishan_ on 5/2/24.
//

#include "IdentifierBinding.hpp"
#include <fmt/format.h>

namespace goos::ast::expression {
  IdentifierBinding::IdentifierBinding(String identifier) : identifier{std::move(identifier)} {}

  const String& IdentifierBinding::get_identifier() const { return identifier; }

  String IdentifierBinding::to_string() const { return identifier; }

  Box<Expression> IdentifierBinding::clone_expr() const {
    return crab::make_box<IdentifierBinding>(identifier);
  }
}
