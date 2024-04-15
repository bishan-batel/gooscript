#include <iostream>
#include <preamble.hpp>

#include "token.hpp"
#include <expected>

int main() {
  std::cout << "huh" << std::endl;

  const Box<Token> token = box::make<KeywordToken>(Keyword::DEFAULT);
  std::cin.get();
}
