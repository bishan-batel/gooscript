//
// Created by bishan_ on 6/13/24.
//

#include "Identifier.hpp"

#include <stdexcept>

goos::meta::Identifier::Identifier(WideString name)
  : name{std::move(name)} {
  compute_hash();
}

auto goos::meta::Identifier::get_string() const -> const WideString& {
  return name;
}

goos::meta::Identifier::operator const std::wstring&() const {
  return get_string();
}

auto goos::meta::Identifier::get_hash() const -> Hash {
  return hash;
}

auto goos::meta::Identifier::operator==(const Identifier &identifier) const -> bool {
  return identifier.get_hash() == get_hash();
}

auto goos::meta::Identifier::compute_hash() -> void {
  hash = std::hash<WideString>()(name);
}

auto std::hash<goos::meta::Identifier>::operator()(const goos::meta::Identifier &ident) const noexcept -> usize {
  return ident.get_hash();
}
