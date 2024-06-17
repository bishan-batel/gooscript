//
// Created by bishan_ on 6/13/24.
//

#include "Identifier.hpp"

#include "runtime/data/GString.hpp"
#include "utils/str.hpp"

goos::meta::Identifier::Identifier(Rc<WideString> name)
  : name{std::move(name)}, hash{utils::hash(this->name)} {}

auto goos::meta::Identifier::wrap(Rc<WideString> name) -> Identifier {
  return Identifier{std::move(name)};
}

auto goos::meta::Identifier::from(WideString name) -> Identifier {
  return wrap(crab::make_rc_mut<WideString>(std::move(name)));
}

auto goos::meta::Identifier::from(const StringView name) -> Identifier {
  return from(str::convert(name));
}

auto goos::meta::Identifier::from(const runtime::GString &name) -> Identifier {
  return wrap(name.get());
}

auto goos::meta::Identifier::get_string() const -> const WideString& {
  return name;
}

auto goos::meta::Identifier::get_string_ref_counted() const -> Rc<WideString> {
  return name;
}

goos::meta::Identifier::operator const WideString&() const {
  return get_string();
}

auto goos::meta::Identifier::get_hash() const -> Hash {
  return hash;
}

auto goos::meta::Identifier::operator==(const Identifier &identifier) const -> bool {
  return identifier.get_hash() == get_hash();
}

auto std::hash<goos::meta::Identifier>::operator()(const goos::meta::Identifier &ident) const noexcept -> usize {
  return ident.get_hash();
}
