//
// Created by bishan_ on 4/21/24.
//

#include "Operator.hpp"

namespace goos::token {
  Operator::Operator(const lexer::Operator op) : op(op) {}

  lexer::Operator Operator::get_op() const { return op; }

  Box<Token> Operator::clone() const {
    return crab::make_box<Operator>(*this);
  }

  String Operator::to_string() const {
    return String{lexer::OPERATOR_TO_STR_MAP.at(op)};
  }
}
