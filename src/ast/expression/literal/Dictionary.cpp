//
// Created by bishan_ on 5/3/24.
//

#include "Dictionary.hpp"

#include <sstream>

namespace goos::ast::expression {
  Dictionary::Dictionary(Vec<Pair> pairs): pairs{std::move(pairs)} {}

  const Vec<Dictionary::Pair>& Dictionary::get_pairs() const { return pairs; }

  WideString Dictionary::to_string() const {
    WideStringStream stream{};

    stream << '{';
    for (const auto &[key, value]: pairs) {
      stream << '(' << key->to_string() << ") => (" << value->to_string() << "), ";
    }
    stream << '}';

    return stream.str();
  }

  Box<Expression> Dictionary::clone_expr() const {
    Vec<Pair> cloned{};
    for (const auto &[key, value]: pairs) {
      cloned.push_back({key->clone_expr(), value->clone_expr()});
    }

    return crab::make_box<Dictionary>(std::move(cloned));
  }
}
