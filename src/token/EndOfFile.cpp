//
// Created by bishan_ on 5/3/24.
//

#include "EndOfFile.hpp"

goos::token::EndOfFile::EndOfFile() = default;

Box<goos::token::Token> goos::token::EndOfFile::clone() const {
  return crab::make_box<EndOfFile>();
}

String goos::token::EndOfFile::to_string() const {
  return "EOF";
}

bool goos::token::EndOfFile::operator==(const Token &other) const {
  return crab::ref::cast<EndOfFile>(other).is_some();
}
