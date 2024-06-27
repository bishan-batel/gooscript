//
// Created by bishan_ on 5/2/24.
//

#include "IdentifierBinding.hpp"
#include <fmt/format.h>

#include "json/Object.hpp"

namespace goos::ast::expression {
  IdentifierBinding::IdentifierBinding(meta::Identifier identifier)
    : identifier{
      crab::make_rc<meta::Identifier>(std::move(identifier))
    } {}

  auto IdentifierBinding::get_identifier() const -> const meta::Identifier& { return identifier; }

  auto IdentifierBinding::to_string() const -> WideString {
    return identifier.get_string();
  }

  auto IdentifierBinding::clone_expr() const -> Box<Expression> {
    return crab::make_box<IdentifierBinding>(identifier);
  }

  auto IdentifierBinding::operator==(const Statement &statement) const -> bool {
    if (auto other = crab::ref::cast<IdentifierBinding>(statement)) {
      return other.take_unchecked()->identifier == identifier;
    }
    return false;
  }

  auto IdentifierBinding::json() const -> Box<json::Value> {
    auto obj = crab::make_box<json::Object>();
    obj->put(L"type", L"identifier");
    obj->put(L"ident", identifier);
    return obj;
  }

  auto IdentifierBinding::accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> {
    return visitor.visit_identifier_binding(*this);
  }
}
