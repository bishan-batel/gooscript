//
// Created by bishan_ on 6/15/24.
//

#include "Dictionary.hpp"

#include <algorithm>
#include <box.hpp>
#include <ranges>

#include "json/Object.hpp"
#include "json/utils.hpp"
#include "numeric"
#include "utils/hash.hpp"
#include "utils/str.hpp"

namespace goos::runtime {
  Dictionary::Dictionary() : cached_hash{0}, is_cache_dirty{true} {}

  auto Dictionary::insert_with_precomputed_hash(const utils::hash_code hash_code, Any key, Any value) -> void {
    // just insert it directly
    pairs.insert_or_assign(hash_code, Pair{std::move(key), std::move(value)});
  }

  auto Dictionary::set(const meta::Identifier &identifier, Any value) -> void {
    set(Any{crab::make_rc_mut<GString>(identifier)}, std::move(value));
  }

  auto Dictionary::set(Any key, Any value) -> void {
    // get the hash on the fly
    insert_with_precomputed_hash(key->hash(), std::move(key), std::move(value));
  }

  auto Dictionary::get(const meta::Identifier &key) const -> Option<Any> {
    Option<Pair> pair_opt = index(utils::hash_together(key.get_hash(), meta::VariantType::STRING));
    if (pair_opt.is_none())
      return crab::none;

    const auto &[_, value] = pair_opt.take_unchecked();
    return crab::some(value);
  }

  auto Dictionary::get(const Any &key) const -> Option<Any> { return get<IValue>(key.get()); }

  auto Dictionary::get_lvalue(const meta::Identifier &key) -> Option<RcMut<LValue>> {
    Option<RefMut<Pair>> pair_opt = index(utils::hash_together(key.get_hash(), meta::VariantType::STRING));
    if (pair_opt.is_none())
      return crab::none;

    auto &[_, value] = *pair_opt.take_unchecked();
    return crab::some(LValue::wrap(value));
  }

  auto Dictionary::get_or_insert_lvalue(const Any &key) -> RcMut<LValue> {
    if (auto opt = get_lvalue(Rc<IValue>{key})) {
      return opt.take_unchecked();
    }

    set(key, Unit::value());

    return get_lvalue(*key).take_unchecked();
  }

  auto Dictionary::index(const Any index) -> Result<Any, Box<err::Error>> { return Any{get_or_insert_lvalue(index)}; }

  auto Dictionary::index(const utils::hash_code hashed_key) const -> Option<Pair> {
    if (not has_key_index(hashed_key)) {
      return crab::none;
    }

    return crab::some(pairs.at(hashed_key));
  }

  auto Dictionary::index(const utils::hash_code hashed_key) -> Option<RefMut<Pair>> {
    if (not has_key_index(hashed_key)) {
      return crab::none;
    }

    return crab::some(RefMut{pairs.at(hashed_key)});
  }

  auto Dictionary::has_key_index(const utils::hash_code hashed_key) const -> bool { return pairs.contains(hashed_key); }

  auto Dictionary::get_pairs() const -> const ::Dictionary<utils::hash_code, Pair> & { return pairs; }

  auto Dictionary::length() const -> usize { return pairs.size(); }

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

  auto Dictionary::get_type() const -> meta::VariantType { return meta::VariantType::OBJECT; }

  auto Dictionary::base_hash() const -> utils::hash_code {
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
    cached_hash = std::ranges::fold_left(pairs, 0, [](utils::hash_code lhs, auto pair) {
      return utils::hash_code_mix(lhs, pair.first, pair.second.second->hash());
    });

    is_cache_dirty = false;
  }
  auto Dictionary::to_json() const -> Box<json::Value> {
    auto obj = crab::make_box<json::Object>();

    for (const auto &[key, value]: pairs | std::views::values) {
      if (auto str = key->coerce<GString>()) {
        obj->put(str.take_unchecked()->get(), value->to_json());
        continue;
      }
      obj->put(key->to_string(), value->to_json());
    }

    return obj;
  }
} // namespace goos::runtime
