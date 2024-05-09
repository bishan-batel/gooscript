//
// Created by bishan_ on 4/25/24.
//

#include "Statement.hpp"

#include <sys/stat.h>

#include "utils/str.hpp"

namespace goos::ast {
  auto Statement::operator!=(const Statement &statement) const -> bool {
    return !(*this == statement);
  }

  auto operator<<(std::ostream &os, const Statement &statement) -> std::ostream& {
    return os << str::convert(statement.to_string());
  }
}
