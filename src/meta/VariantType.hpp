//
// Created by bishan_ on 4/25/24.
//

#pragma once
#include "utils/hash.hpp"

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
    REFERENCE,
    ANY
  };

  inline auto operator <<(std::ostream &os, const VariantType ty) -> std::ostream& {
    switch (ty) {
      case VariantType::INTEGER: {
        os << "integer";
        break;
      }
      case VariantType::DECIMAL: {
        os << "decimal";
        break;
      }
      case VariantType::STRING: {
        os << "string";
        break;
      }
      case VariantType::BOOLEAN: {
        os << "boolean";
        break;
      }
      case VariantType::OBJECT: {
        os << "object";
        break;
      }
      case VariantType::ARRAY: {
        os << "array";
        break;
      }
      case VariantType::FUNCTION: {
        os << "function";
        break;
      }
      case VariantType::NIL: {
        os << "nil";
        break;
      }
      case VariantType::UNIT: {
        os << "unit";
        break;
      }
      case VariantType::REFERENCE: {
        os << "reference";
        break;
      }
      case VariantType::ANY: {
        os << "any";
        break;
      }
    }

    return os;
  }
}

template<>
struct std::hash<goos::meta::VariantType> {
  constexpr auto operator()(goos::meta::VariantType ty) const noexcept -> usize {
    using namespace goos;
    return utils::hash(static_cast<std::underlying_type_t<meta::VariantType>>(ty));
  }
};
