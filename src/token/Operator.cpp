//
// Created by bishan_ on 4/21/24.
//

#include "Operator.hpp"

namespace goos::token {
  Operator::Operator(const lexer::Operator op) : op(op) {}

  auto Operator::get_op() const -> lexer::Operator { return op; }

  auto Operator::clone() const -> Box<Token> {
    return crab::make_box<Operator>(op);
  }

  auto Operator::to_string() const -> WideString {
    return WideString{lexer::OPERATOR_TO_STR_MAP.at(op)};
  }

  auto Operator::operator==(const Token &other) const -> bool {
    if (const Option<Ref<Operator>> op = crab::ref::cast<Operator>(other)) {
      return op.get_unchecked()->op == this->op;
    }
    return false;
  }
}
