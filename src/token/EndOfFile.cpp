//
// Created by bishan_ on 5/3/24.
//

#include "EndOfFile.hpp"

goos::token::EndOfFile::EndOfFile() = default;

auto goos::token::EndOfFile::clone() const -> Box<Token> {
  return crab::make_box<EndOfFile>();
}

auto goos::token::EndOfFile::to_string() const -> WideString {
  return L"EOF";
}

auto goos::token::EndOfFile::operator==(const Token &other) const -> bool {
  return crab::ref::cast<EndOfFile>(other).is_some();
}
