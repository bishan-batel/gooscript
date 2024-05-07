//
// Created by bishan_ on 5/2/24.
//

#include "IdentifierBinding.hpp"
#include <fmt/format.h>

namespace goos::ast::expression {
  IdentifierBinding::IdentifierBinding(WideString identifier) : identifier{std::move(identifier)} {}

  const WideString& IdentifierBinding::get_identifier() const { return identifier; }

  WideString IdentifierBinding::to_string() const { return identifier; }

  Box<Expression> IdentifierBinding::clone_expr() const {
    return crab::make_box<IdentifierBinding>(identifier);
  }

  bool IdentifierBinding::operator==(const Statement &statement) const {
    if (auto other = crab::ref::cast<IdentifierBinding>(statement)) {
      return other.take_unchecked()->identifier == identifier;
    }
    return false;
  }
}
