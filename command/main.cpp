#include <fstream>

#include "preamble.hpp"
#include "lexer/Lexer.hpp"
#include "parser/TokenStream.hpp"
#include "parser/pass/statement/block.hpp"
#include "runtime/Intepreter.hpp"
#include "runtime/data/Value.hpp"
#include "source/SourceFile.hpp"
#include <fmt/format.h>
#include <fmt/color.h>
#include <fmt/xchar.h>

#include "utils/str.hpp"

template<typename F> requires std::is_invocable_r_v<Vec<String>, F, String&>
auto parse(Vec<String> args, F word) -> Dictionary<String, Vec<String>> {
  Dictionary<String, Vec<String>> result;

  String flag;
  for (auto &&x: args) {
    if (auto f = word(x); f.empty()) {
      result[flag].push_back(x);
    } else {
      flag = f.front();
      result[flag];
      flag = f.back();
    }
  }
  return result;
}

auto parse(const i32 argc, const char *argv[]) -> Dictionary<String, Vec<String>> {
  const Vec<String> args(argv + 1, argv + argc);

  Dictionary<String, String> keys = {
    {"-h", "help"},
    {"--h", "help"},
    {"-t", "tokens"},
    {"--tokens", "tokens"},
    {"--emit-ast", "emit-ast"},
    {"--o", "emit-ast"},
    {"-a", "emit-ast"},

    {"-D", "emit-args"},
    {"--emit-args", "emit-args"},
  };

  auto result = parse(
    args,
    [&](auto &&s) -> Vec<String> {
      if (keys.contains(s))
        return {keys[s]};
      return {};
    }
  );
  result["__executable__"].push_back(argv[0]);
  return result;
}

auto main(i32 argc, const char *argv[]) -> i32 {
  using namespace goos;

  const auto args = parse(argc, argv);

  if (args.contains("emit-args")) {
    for (const auto &[k, v]: args) {
      print(fg(fmt::color::light_green), "\"{}\"", k);
      print(fg(fmt::color::light_gray), ": [ ");
      for (const auto &s: v) {
        print(fg(fmt::color::light_green), "\"{}\" ", s);
      }
      print(fg(fmt::color::light_gray), "]\n");
    }
    return 0;
  }

  if (not args.contains("") or args.at("").empty() or args.contains("help")) {
    print(fg(fmt::color::light_green) | fmt::emphasis::bold, "[USAGE]: ");
    fmt::print("goos ");
    print(fg(fmt::color::yellow), "{{FILE_NAME}} ");
    print(fg(fmt::color::light_sky_blue), "{{FLAGS}}");
    fmt::println("");
    return 1;
  }

  const String filepath{args.at("")[0]};
  auto file_result = SourceFile::from_file(filepath);
  if (file_result.is_err()) {
    io::Error err{file_result.take_err_unchecked()};

    print(stderr, fg(fmt::color::red), "[ERROR]: ");
    print(stderr, fg(fmt::color::light_coral), "Could not open file: {}", err.what());
    return 1;
  }

  const auto file{file_result.take_unchecked()};
  lexer::TokenList list{lexer::Lexer::tokenize(file).take_unchecked()};

  if (args.contains("tokens")) {
    print(fg(fmt::color::light_gray), "Tokens: \n");
    for (const auto &tok: list) {
      print(fg(fmt::color::light_gray), "\t{}\n", str::convert(tok->to_string()));
    }
  }

  parser::TokenStream stream{std::move(list)};

  auto result = parser::pass::block_top_level(stream);

  if (result.is_err()) {
    const parser::err::Error err{result.take_err_unchecked()};

    print(stderr, fg(fmt::color::red), "[ERROR]: ");
    print(stderr, fg(fmt::color::pale_violet_red), "Syntax Error: ");
    print(stderr, fg(fmt::color::light_coral), "{}\n", err.what());

    const auto &tok = err.get().where();

    const auto [line, col] = tok.get_line_and_column();

    const auto line_range{file.get_line(line)};
    print(stderr, fg(fmt::color::light_coral), "[{}]: {}\n", line, str::convert(file.slice(line_range)));
    print(stderr, fg(fmt::color::light_coral), "[{}]: ", line);

    const auto range = tok.get_range();
    for (usize i: line_range) {
      if (range.lower_bound() <= i and range.upper_bound() > i) {
        print(stderr, fg(fmt::color::pale_violet_red), "^");
        continue;
      }
      fmt::print(stderr, " ");
    }

    return 1;
  }

  if (args.contains("emit-ast")) {
    const Vec<String> &ast_args{args.at("emit-ast")};
    const String ast_filepath{ast_args.empty() ? fmt::format("{}.json", filepath) : ast_args[0]};

    std::wofstream output{ast_filepath};
    print(fg(fmt::color::light_gray), "Emitted AST to: '");
    print(fg(fmt::color::light_coral), "{}", ast_filepath);
    print(fg(fmt::color::light_gray), "'\n");
    result.get_unchecked()->json()->write(output);
  }

  print(fg(fmt::color::light_green), "Running Program: \n");
  runtime::Intepreter intepreter{};
  Rc<runtime::Value> val = intepreter.evaluate(result.take_unchecked()).take_unchecked();

  std::wcout << "Program exited with value: " << val->to_string() << std::endl;
  return 0;
}
