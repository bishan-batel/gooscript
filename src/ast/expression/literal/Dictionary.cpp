//
// Created by bishan_ on 5/3/24.
//

#include "Dictionary.hpp"

#include <sstream>

namespace goos::ast::expression {
  Dictionary::Dictionary(Vec<Pair> pairs): pairs{std::move(pairs)} {}

  auto Dictionary::get_pairs() const -> const Vec<Pair>& { return pairs; }

  auto Dictionary::get(const Expression &key) const -> Option<Ref<Pair>> {
    for (const auto &pair: pairs) {
      if (*pair.key == key) return crab::some(Ref{pair});
    }
    return crab::none;
  }

  auto Dictionary::to_string() const -> WideString {
    WideStringStream stream{};

    stream << '{';
    for (const auto &[key, value]: pairs) {
      stream << '(' << key->to_string() << ") => (" << value->to_string() << "), ";
    }
    stream << '}';

    return stream.str();
  }

  auto Dictionary::clone_expr() const -> Box<Expression> {
    Vec<Pair> cloned{};
    for (const auto &[key, value]: pairs) {
      cloned.push_back({key->clone_expr(), value->clone_expr()});
    }

    return crab::make_box<Dictionary>(std::move(cloned));
  }

  auto Dictionary::operator==(const Statement &statement) const -> bool {
    auto other_opt = crab::ref::cast<Dictionary>(statement);
    if (other_opt.is_none()) return false;
    const Ref<Dictionary> other{other_opt.take_unchecked()};

    if (pairs.size() != other->pairs.size()) return false;

    for (const auto &[key, value]: pairs) {
      if (auto other_value = other->get(key)) {
        if (*value != *other_value.take_unchecked()->value) return false;
      } else {
        return false;
      }
    }
    return true;
  }
}
