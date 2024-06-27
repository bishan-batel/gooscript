//
// Created by bishan_ on 6/15/24.
//

#include "PropertyAccess.hpp"
#include <fmt/xchar.h>

#include "json/Object.hpp"

namespace goos::ast::expression {
  PropertyAccess::PropertyAccess(Box<Expression> object, meta::Identifier property)
    : object{std::move(object)},
      property{std::move(property)} {}

  auto PropertyAccess::get_object() const -> const Expression& {
    return object;
  }

  auto PropertyAccess::get_property() const -> meta::Identifier {
    return property;
  }

  auto PropertyAccess::operator==(const Statement &statement) const -> bool {
    if (auto other = statement.try_as<PropertyAccess>()) {
      const PropertyAccess &access = other.take_unchecked();
      return *access.object == *object and access.property == property;
    }
    return false;
  }

  auto PropertyAccess::to_string() const -> WideString {
    return fmt::format(L"({}).{}", object->to_string(), property.get_string());
  }

  auto PropertyAccess::json() const -> Box<json::Value> {
    auto json = crab::make_box<json::Object>();

    json->put(L"object", object->json());
    json->put(L"property", property);

    return json;
  }

  auto PropertyAccess::clone_expr() const -> Box<Expression> {
    return crab::make_box<PropertyAccess>(object->clone_expr(), property);
  }

  auto PropertyAccess::accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> {
    return visitor.visit_property_access(*this);
  }
} // runtime
