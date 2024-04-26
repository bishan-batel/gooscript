#include <fstream>
#include <iostream>
#include <sstream>

#include <fmt/core.h>

#include "pattern_match.hpp"
#include "preamble.hpp"
#include "lexer/Lexer.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Lexer", "What") {}

int main() {
  const Rc content{(std::stringstream{} << std::ifstream{"examples/syntax.goo"}.rdbuf()).str()};;

  if_ok(
    goos::lexer::Lexer::tokenize(content),
    [](const goos::lexer::TokenList &list) {
      for (const auto &token: list) {
        fmt::print("('{}') ", token->to_string());
      }
    }
  ).or_else(
    [](auto e) {
      fmt::println(stderr, "ERROR:\n{}", e.what());
    }
  );

  // std::cin.get();
}
