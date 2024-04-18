#include <iostream>
#include <preamble.hpp>

#include "src/lexer/Token.hpp"
#include <expected>

int main() {
  std::cout << "huh" << std::endl;

  const Box<lexer::Token> token =
      box::make<lexer::KeywordToken>(lexer::Keyword::DEFAULT);
  std::cin.get();
}
