//
// Created by bishan_ on 6/14/24.
//

#include "PrimitiveOperations.hpp"

namespace goos::runtime::primitive_operators {} // goos

auto std::hash<goos::runtime::primitive_operators::BinaryOperands>::operator()(
  const goos::runtime::primitive_operators::BinaryOperands &ty
) const noexcept -> usize {
  return goos::utils::hash_together(ty.lhs, ty.rhs, ty.op);
}

auto std::hash<goos::runtime::primitive_operators::UnaryOperands>::operator()(
  const goos::runtime::primitive_operators::UnaryOperands &ty
) const noexcept -> usize {
  return goos::utils::hash_together(ty.op, ty.operand);
}

namespace goos::runtime::primitive_operators {
  auto BinaryOperands::operator==(const BinaryOperands &other) const -> bool {
    return lhs == other.lhs and rhs == other.rhs and op == other.op;
  }

  auto UnaryOperands::operator==(const UnaryOperands &other) const -> bool {
    return op == other.op and operand == other.operand;
  }
}
