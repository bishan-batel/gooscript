//
// Created by bishan_ on 5/3/24.
//

#include "EndOfFile.hpp"

goos::token::EndOfFile::EndOfFile(SourceFile file)
  : Token{std::move(file), crab::range<usize>(0, 1)} {}

auto goos::token::EndOfFile::to_string() const -> WideString {
  return L"EOF";
}

auto goos::token::EndOfFile::operator==(const Token &other) const -> bool {
  return crab::ref::cast<EndOfFile>(other).is_some();
}
