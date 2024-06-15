//
// Created by bishan_ on 6/14/24.
//

#include "PrimitiveOperations.hpp"

namespace goos::runtime::primitive_operators {} // goos

auto std::hash<goos::runtime::primitive_operators::Operands>::operator()(
  const goos::runtime::primitive_operators::Operands &ty
) const noexcept -> usize {
  return static_cast<usize>(ty.lhs) * 2 + static_cast<usize>(ty.rhs) * 3 + static_cast<usize>(ty.op) * 5;
}

auto goos::runtime::primitive_operators::Operands::operator==(
  const Operands &other
) const -> bool {
  return lhs == other.lhs and rhs == other.rhs and op == other.op;
}
