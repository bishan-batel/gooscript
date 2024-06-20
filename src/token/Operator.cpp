//
// Created by bishan_ on 4/21/24.
//

#include "Operator.hpp"
#include <fmt/format.h>
#include <fmt/xchar.h>

namespace goos::token {
  Operator::Operator(SourceFile file, const Range<> range, const lexer::Operator op)
    : Token{std::move(file), range}, op{op} {}

  auto Operator::get_op() const -> lexer::Operator { return op; }

  auto Operator::to_string() const -> WideString {
    return fmt::format(L"Operator {{'{}'}}", lexer::OPERATOR_TO_STR_MAP.at(op));
  }

  auto Operator::operator==(const Token &other) const -> bool {
    if (const Option<Ref<Operator>> op = crab::ref::cast<Operator>(other)) {
      return op.get_unchecked()->op == this->op;
    }
    return false;
  }
}
