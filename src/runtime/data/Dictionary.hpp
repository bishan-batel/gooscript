//
// Created by bishan_ on 6/15/24.
//

#pragma once
#include "IValue.hpp"
#include "TypeConversion.hpp"
#include "ast/Statement.hpp"
#include "meta/Identifier.hpp"

namespace goos::runtime {
  class Dictionary final : public IValue {
  public:
    using Pair = std::pair<Rc<IValue>, Any>;

  private:
    ::Dictionary<usize, Pair> pairs;

    mutable usize cached_hash{0};
    mutable bool is_cache_dirty{true};

  public:
    Dictionary();

    auto insert_with_precomputed_hash(usize hash_code, Any key, Any value) -> void;

    /**
     * @brief Sets the according key with the given value, if the key already exists it will not be
     * replaced / inserted, if it does not exist the key-value pair will be inserted into the dictionary.
     */
    template<typename Key, typename Value=IValue> requires type::is_value<Key> and type::is_value<Value>
    auto set(RcMut<Key> key, RcMut<Value> value) -> void;

    auto set(const meta::Identifier &identifier, Any value) -> void;

    auto set(WideStringView key, Any value) -> void;

    /**
     * @brief Sets the according key with the given value, if the key already exists it will not be
     * replaced / inserted, if it does not exist the key-value pair will be inserted into the dictionary.
     */
    auto set(Any key, Any value) -> void;

    template<typename T=IValue> requires type::is_value<T>
    auto get(const T &key) const -> Option<Any>;

    template<typename T=IValue> requires type::is_value<T>
    auto get(const RcMut<T> &key) const -> Option<Any>;

    auto get(const meta::Identifier &key) const -> Option<Any>;

    auto index(usize hashed_key) const -> Option<Ref<Pair>>;

    template<typename T=IValue> requires type::is_value<T>
    auto has_key(const T &hashed_key) const -> bool;

    auto has_key_index(usize hashed_key) const -> bool;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    [[nodiscard]] auto base_hash() const -> usize override;

    [[nodiscard]] auto clone() const -> Any override;

    auto force_recompute_hash() const -> void;
  };

  template<typename Key, typename Value> requires type::is_value<Key> && type::is_value<Value>
  auto Dictionary::set(RcMut<Key> key, RcMut<Value> value) -> void {
    set(*key, Any{value});
  }

  template<typename T> requires type::is_value<T>
  auto Dictionary::get(const T &key) const -> Option<Any> {
    auto pair = index(key->hash());

    if (pair.is_none()) return crab::none;

    const auto &[_, value] = *pair.take_unchecked();

    return crab::some(value);
  }

  template<typename T> requires type::is_value<T>
  auto Dictionary::get(const RcMut<T> &key) const -> Option<Any> {
    return get<T>(*key);
  }

  template<typename T> requires type::is_value<T>
  auto Dictionary::has_key(const T &hashed_key) const -> bool {
    return has_key(hashed_key->hash());
  }
} // goos
