//
// Created by bishan_ on 4/25/24.
//

#include "Integer.hpp"

namespace goos::token {
  Integer::Integer(const i64 number) : number{number} {}

  i64 Integer::get_number() const { return number; }

  Box<Token> Integer::clone() const { return crab::make_box<Integer>(*this); }

  String Integer::to_string() const { return fmt::format("{}", number); }
}
