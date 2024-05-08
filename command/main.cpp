#include <fstream>

#include "preamble.hpp"
#include "ast/expression/literal/Array.hpp"
#include "lexer/Lexer.hpp"
#include "parser/TokenStream.hpp"
#include "parser/pass/statement/block.hpp"
#include "source/SourceFile.hpp"

auto main(i32 argc, const char *argv[]) -> i32 {
  if (argc < 3) {
    std::cout << "USAGE: goos [FILE_NAME] [AST_OUTPUT]" << std::endl;;
    return 1;
  }

  const goos::SourceFile file = goos::SourceFile::from_file(argv[1]).take_unchecked();
  goos::parser::TokenStream stream{goos::lexer::Lexer::tokenize(file).take_unchecked()};

  auto result = goos::parser::pass::block_top_level(stream).take_unchecked();

  std::wofstream output{argv[2]};
  result->json()->write(output);
  return 0;
}
