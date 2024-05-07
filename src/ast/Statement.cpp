//
// Created by bishan_ on 4/25/24.
//

#include "Statement.hpp"

#include <sys/stat.h>

#include "utils/str.hpp"

auto goos::ast::Statement::operator!=(const Statement &statement) const -> bool {
  return !(*this == statement);
}

auto goos::ast::operator<<(std::ostream &os, const Statement &statement) -> std::ostream& {
  return os << str::convert(statement.to_string());
}
