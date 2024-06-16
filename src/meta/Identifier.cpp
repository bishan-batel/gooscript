//
// Created by bishan_ on 6/13/24.
//

#include "Identifier.hpp"

#include "utils/str.hpp"

goos::meta::Identifier::Identifier(WideString name)
  : name{crab::make_rc<WideString>(std::move(name))} {
  compute_hash();
}

auto goos::meta::Identifier::get_string() const -> const WideString& {
  return name;
}

auto goos::meta::Identifier::get_string_ref_counted() const -> Rc<WideString> {
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
