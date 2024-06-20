//
// Created by bishan_ on 4/25/24.
//

#include "Integer.hpp"
#include <fmt/xchar.h>

namespace goos::token {

  Integer::Integer(SourceFile file, const Range<> range, const i64 number)
    : Token{std::move(file), range}, number{number} {}

  auto Integer::get_number() const -> i64 { return number; }

  auto Integer::to_string() const -> WideString { return fmt::format(L"{}", number); }

  auto Integer::operator==(const Token &other) const -> bool {
    if (const Option<Ref<Integer>> num = crab::ref::cast<Integer>(other)) {
      return num.get_unchecked()->number == number;
    }
    return false;

  }
}
