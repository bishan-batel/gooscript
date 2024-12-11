//
// Created by bishan_ on 6/15/24.
//

#pragma once
#include "ExternFunction.hpp"
#include "interfaces/IValue.hpp"
#include "LValue.hpp"
#include "TypeConversion.hpp"
#include "ast/Statement.hpp"
#include "meta/Identifier.hpp"

namespace goos::runtime {
  class Dictionary final : public IIndexible {
  public:
    using Pair = std::pair<RcMut<IValue>, Any>;

  private:
    ::Dictionary<utils::hash_code, Pair> pairs;

    mutable utils::hash_code cached_hash{0};
    mutable bool is_cache_dirty{true};

  public:
    static constexpr meta::VariantType TYPE{meta::VariantType::OBJECT};

    Dictionary();

    auto insert_with_precomputed_hash(utils::hash_code hash_code, Any key, Any value) -> void;

    /**
     * @brief Sets the according key with the given value, if the key already exists it will not be
     * replaced / inserted, if it does not exist the key-value pair will be inserted into the dictionary.
     */
    template<typename Key, typename Value=IValue> requires type::value_type<Key> and type::value_type<Value>
    auto set(RcMut<Key> key, RcMut<Value> value) -> void;

    auto set(const meta::Identifier &identifier, Any value) -> void;

    template<typename IntoIdentifier, type::value_type ValueTy>
    auto set(IntoIdentifier identifier, RcMut<ValueTy> value) -> void;

    template<typename IntoIdentifier, type::convertible_primitive IntoValue>
    auto set(IntoIdentifier identifier, IntoValue value) -> void;

    template<typename IntoIdentifier, typename F>
    auto set_func(IntoIdentifier name, F function) -> void;

    /**
     * @brief Sets the according key with the given value, if the key already exists it will not be
     * replaced / inserted, if it does not exist the key-value pair will be inserted into the dictionary.
     */
    auto set(Any key, Any value) -> void;

    template<type::value_type T=IValue>
    auto get(const T &key) const -> Option<Any>;

    template<type::value_type T=IValue>
    auto get(const Rc<T> &key) const -> Option<Any>;

    auto get(const meta::Identifier &key) const -> Option<Any>;

    auto get(const Any &key) const -> Option<Any>;

    template<type::value_type T=IValue>
    auto get_lvalue(const T &key) -> Option<RcMut<LValue>>;

    template<type::value_type T=IValue>
    auto get_lvalue(const Rc<T> &key) -> Option<RcMut<LValue>>;

    auto get_lvalue(const meta::Identifier &key) -> Option<RcMut<LValue>>;

    auto get_or_insert_lvalue(const Any &key) -> RcMut<LValue>;

    [[nodiscard]] auto index(Any index) -> Result<Any, Box<err::Error>> override;

    auto index(utils::hash_code hashed_key) const -> Option<Pair>;

    auto index(utils::hash_code hashed_key) -> Option<RefMut<Pair>>;

    template<type::value_type T = IValue>
    auto has_key(const T &hashed_key) const -> bool;

    auto has_key_index(utils::hash_code hashed_key) const -> bool;

    [[nodiscard]] auto get_pairs() const -> const ::Dictionary<utils::hash_code, Pair>&;

    [[nodiscard]] auto length() const -> usize;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    [[nodiscard]] auto base_hash() const -> utils::hash_code override;

    [[nodiscard]] auto clone() const -> Any override;

    auto force_recompute_hash() const -> void;

    [[nodiscard]] auto to_json() const -> Box<json::Value> override;
  };

  template<typename>
  struct is_dictionary_row {
    static constexpr auto value = false;
  };

  template<typename K, typename V> requires (std::is_constructible_v<meta::Identifier, K> && (
                                               type::convertible_primitive<K> or type::value_type<V>))
  struct is_dictionary_row<std::pair<K, V>> {
    static constexpr auto value = true;
  };

  template<typename... Args> requires (is_dictionary_row<Args>::value and ... )
  auto dict(Args... args) -> RcMut<Dictionary> {
    auto dict = crab::make_rc_mut<Dictionary>();

    (void(dict->set(args.first, Any{args.second})), ...);

    return dict;
  }

  template<typename Key, typename Value> requires type::value_type<Key> && type::value_type<Value>
  auto Dictionary::set(RcMut<Key> key, RcMut<Value> value) -> void {
    set(Any{key}, Any{value});
  }

  template<typename IntoIdentifier, type::value_type ValueTy>
  auto Dictionary::set(IntoIdentifier identifier, RcMut<ValueTy> value) -> void {
    set(meta::Identifier::from(identifier), Any{value});
  }

  template<typename IntoIdentifier, type::convertible_primitive IntoValue>
  auto Dictionary::set(IntoIdentifier identifier, IntoValue value) -> void {
    set(identifier, type::to_goos_any(value));
  }

  template<typename IntoIdentifier, typename F>
  auto Dictionary::set_func(IntoIdentifier name, const F function) -> void {
    set(meta::Identifier::from(std::move(name)), lambda::from(function));
  }

  template<type::value_type T>
  auto Dictionary::get(const T &key) const -> Option<Any> {
    auto pair = index(key.hash());

    if (pair.is_none()) return crab::none;

    const auto &[_, value] = pair.take_unchecked();
    return crab::some(value);
  }

  template<type::value_type T>
  auto Dictionary::get(const Rc<T> &key) const -> Option<Any> {
    return this->get<T>(*key);
  }

  template<type::value_type T>
  auto Dictionary::get_lvalue(const T &key) -> Option<RcMut<LValue>> {
    auto pair = index(key.hash());

    if (pair.is_none()) return crab::none;

    auto &[_, value] = *pair.take_unchecked();

    return crab::some(LValue::wrap(value));
  }

  template<type::value_type T>
  auto Dictionary::get_lvalue(const Rc<T> &key) -> Option<RcMut<LValue>> {
    return this->get_lvalue<T>(*key);
  }

  template<type::value_type T>
  auto Dictionary::has_key(const T &hashed_key) const -> bool {
    return this->has_key(hashed_key->hash());
  }
} // goos
