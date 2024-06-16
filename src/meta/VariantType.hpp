//
// Created by bishan_ on 4/25/24.
//

#pragma once

namespace goos::meta {
  enum class VariantType {
    INTEGER,
    DECIMAL,
    STRING,
    BOOLEAN,
    OBJECT,
    ARRAY,
    FUNCTION,
    NIL,
    UNIT,
  };

  inline auto operator <<(std::ostream &os, const VariantType ty) -> std::ostream& {
    switch (ty) {
      case VariantType::INTEGER: os << "integer";
      case VariantType::DECIMAL: os << "decimal";
      case VariantType::STRING: os << "string";
      case VariantType::BOOLEAN: os << "boolean";
      case VariantType::OBJECT: os << "object";
      case VariantType::ARRAY: os << "array";
      case VariantType::FUNCTION: os << "function";
      case VariantType::NIL: os << "nil";
      case VariantType::UNIT: os << "unit";
    }

    return os;
  }
}
