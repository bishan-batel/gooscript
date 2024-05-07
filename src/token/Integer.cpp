//
// Created by bishan_ on 4/25/24.
//

#include "Integer.hpp"
#include <fmt/xchar.h>

namespace goos::token {
  Integer::Integer(const i64 number) : number{number} {}

  i64 Integer::get_number() const { return number; }

  Box<Token> Integer::clone() const { return crab::make_box<Integer>(number); }

  WideString Integer::to_string() const { return fmt::format(L"{}", number); }

  bool Integer::operator==(const Token &other) const {
    if (const Option<Ref<Integer>> num = crab::ref::cast<Integer>(other)) {
      return num.get_unchecked()->number == number;
    }
    return false;

  }
}
