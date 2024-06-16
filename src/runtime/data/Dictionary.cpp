//
// Created by bishan_ on 6/15/24.
//

#include "Dictionary.hpp"

#include "utils/hash.hpp"
#include "utils/str.hpp"

namespace goos::runtime {
  Dictionary::Dictionary() {
    cached_hash = base_hash();
  }

  auto Dictionary::insert_with_precomputed_hash(const usize hash_code, Any key, Any value) -> void {
    pairs.insert_or_assign(hash_code, Pair{std::move(key), std::move(value)});
  }

  auto Dictionary::set(const meta::Identifier &identifier, Any value) -> void {
    insert_with_precomputed_hash(
      identifier.get_hash(),
      crab::make_rc_mut<GString>(identifier),
      std::move(value)
    );
  }

  auto Dictionary::set(Any key, Any value) -> void {
    insert_with_precomputed_hash(key->base_hash(), std::move(key), std::move(value));
  }

  auto Dictionary::get(const meta::Identifier &key) const -> Option<Any> {
    auto pair = index(key.get_hash());
    if (pair.is_none()) return crab::none;

    return crab::some(pair.take_unchecked()->second);
  }

  auto Dictionary::index(const usize hashed_key) const -> Option<Ref<Pair>> {
    if (has_key_index(hashed_key)) {
      return crab::none;
    }

    return crab::some(Ref{pairs.at(hashed_key)});
  }

  auto Dictionary::has_key_index(const usize hashed_key) const -> bool {
    return pairs.contains(hashed_key);
  }

  auto Dictionary::to_string() const -> WideString {
    WideStringStream stream{};

    stream << '{';

    for (const auto &[hash, pair]: pairs) {
      auto [key, value] = pair;

      if (key.downcast<GString>()) {
        stream << '"' << key->to_string() << '"';
      } else {
        stream << key->to_string();
      }
      stream << ": ";

      if (value.downcast<GString>()) {
        stream << '"' << value->to_string() << '"';
      } else {
        stream << value->to_string();
      }

      stream << ", ";
    }

    stream << '}';
    return stream.str();
  }

  auto Dictionary::get_type() const -> meta::VariantType {
    return meta::VariantType::OBJECT;
  }

  auto Dictionary::base_hash() const -> usize {
    if (is_cache_dirty) {
      force_recompute_hash();
    }

    return cached_hash;
  }

  auto Dictionary::clone() const -> Any {
    auto copy = crab::make_rc_mut<Dictionary>();

    for (const auto &[key_hash, pair]: pairs) {
      const auto &[key, value] = pair;

      copy->insert_with_precomputed_hash(key_hash, key->clone(), value->clone());
    }

    return copy;
  }

  auto Dictionary::force_recompute_hash() const -> void {
    usize hash = 0;

    for (const auto &[key_hash, pair]: pairs) {
      const auto &[_, value] = pair;
      hash = utils::combine_hash(hash, key_hash);
      hash = utils::combine_hash(hash, value->base_hash());
    }

    cached_hash = hash;

    is_cache_dirty = false;
  }
}
