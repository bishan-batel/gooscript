//
// Created by bishan_ on 4/25/24.
//

#include "Statement.hpp"

#include <sys/stat.h>

#include "utils/str.hpp"

bool goos::ast::Statement::operator!=(const Statement &statement) const {
  return !(*this == statement);
}

std::ostream& goos::ast::operator<<(std::ostream &os, const Statement &statement) {
  return os << str::convert(statement.to_string());
}
