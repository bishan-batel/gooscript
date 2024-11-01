#include <fstream>

#include "preamble.hpp"
#include "lexer/Lexer.hpp"
#include "parser/TokenStream.hpp"
#include "parser/pass/statement/block.hpp"
#include "runtime/Intepreter.hpp"
#include "../src/runtime/data/interfaces/IValue.hpp"
#include <chrono>
#include "runtime/data/TypeConversion.hpp"
#include "source/SourceFile.hpp"
#include <fmt/format.h>
#include <fmt/color.h>
#include <fmt/xchar.h>

#include "codegen/Compiler.hpp"
#include "utils/str.hpp"
#include "vm/Chunk.hpp"
#include "vm/Vm.hpp"

#include "result.hpp"

auto chunk_test() -> void;

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
    {"-b", "benchmark"},
    {"--benchmark", "benchmark"},
    {"--tokens", "tokens"},
    {"--emit-ast", "emit-ast"},
    {"--o", "emit-ast"},
    {"-a", "emit-ast"},
    {"--vm-test", "vm-test"},

    {"-vm", "bytecode"},

    {"-D", "emit-args"},
    {"--emit-args", "emit-args"},

    {"--vm-emit", "vm-emit-bytecode"},
    {"-bc", "vm-emit-bytecode"},
  };

  auto result = parse(
    args,
    [&](auto &&s) -> Vec<String> {
      if (keys.contains(s))
        return {keys[s]};
      return {};
    }
  );
  result["__executable__"].emplace_back(argv[0]);
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

  if (args.contains("vm-test")) {
    chunk_test();
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

  if (args.contains("bytecode")) {
    using namespace codegen;
    Compiler compiler{L"Bytecode"};

    vm::Chunk chunk{
      compiler.compile(result.take_unchecked()).map(
        [&compiler](auto) {
          return compiler.finalize_chunk();
        }
      ).take_unchecked()
    };

    if (args.contains("vm-emit-bytecode")) {
      const Vec<String> &emit_args{args.at("vm-emit-bytecode")};
      const String byte_filepath{emit_args.empty() ? fmt::format("{}.obj", filepath) : emit_args[0]};

      std::wofstream output{byte_filepath};
      print(fg(fmt::color::light_gray), "Emitted bytecode to: '");
      print(fg(fmt::color::light_coral), "{}", byte_filepath);
      print(fg(fmt::color::light_gray), "'\n");
      chunk.dissassemble();
    }

    // chunk.write_instruction(vm::op::Code::PRINT);

    if (not args.contains("benchmark")) {
      vm::Vm vm{std::move(chunk)};

      const vm::Value val = vm.run().take_unchecked();
      std::cout << "Exited with value " << val.match(
        []<typename T>(T t) -> String requires requires { std::to_string(t); } { return std::to_string(t); },
        []<std::convertible_to<String> T>(T t) -> String { return static_cast<String>(t); },
        [&val](auto) -> String { return String{val.type_name()}; }
      ) << std::endl;

      return 0;
    }

    using namespace std::chrono;

    const usize benchmark_count = args.at("benchmark").empty() ? 1 : std::stoull(args.at("benchmark").at(0));

    nanoseconds total = 0ns;

    const auto padding{str::convert(str::repeat(L' ', 2))};

    for (usize i: crab::range(benchmark_count)) {
      fmt::print("\rRunning benchmark ({}/{}) ...{}", i, benchmark_count, padding);
      std::flush(std::cout);

      vm::Vm vm{chunk};
      auto start = steady_clock::now();
      std::ignore = vm.run().take_unchecked();
      total += steady_clock::now() - start;
    }

    const auto elapsed = total / benchmark_count;

    const seconds sec{std::chrono::duration_cast<seconds>(elapsed)};
    const milliseconds milli{std::chrono::duration_cast<milliseconds>(elapsed) % 1s};
    const microseconds micro{std::chrono::duration_cast<microseconds>(elapsed) % 1ms};
    const nanoseconds nano{elapsed % 1us};

    std::cout << "\rFinished benchmark with an average of: ";

    if (sec >= 0s) {
      std::cout << sec << " ";
    }

    std::cout << milli << " " << micro << " " << nano << std::endl;

    return 0;
  }

  using namespace runtime;

  if (not args.contains("benchmark")) {
    print(fg(fmt::color::light_green), "Running Program: \n");
    Intepreter intepreter{};
    Any exit_result = intepreter.evaluate(result.take_unchecked()).take_unchecked();
    std::wcout << "Program exited with value: " << exit_result->to_string() << std::endl;
    return 0;
  }

  using namespace std::chrono;

  const usize benchmark_count = args.at("benchmark").empty() ? 1 : std::stoull(args.at("benchmark").at(0));

  nanoseconds total = 0ns;

  const auto padding{str::convert(str::repeat(L' ', 2))};

  const auto ast_file{result.take_unchecked()};

  for (usize i: crab::range(benchmark_count)) {
    fmt::print("\rRunning benchmark ({}/{}) ...{}", i, benchmark_count, padding);
    std::flush(std::cout);

    Intepreter intepreter{};
    auto start = steady_clock::now();
    std::ignore = intepreter.evaluate(ast_file).take_unchecked();
    total += steady_clock::now() - start;
  }

  const auto elapsed = total / benchmark_count;

  const seconds sec{std::chrono::duration_cast<seconds>(elapsed)};
  const milliseconds milli{std::chrono::duration_cast<milliseconds>(elapsed) % 1s};
  const microseconds micro{std::chrono::duration_cast<microseconds>(elapsed) % 1ms};
  const nanoseconds nano{elapsed % 1us};

  std::cout << "\rFinished benchmark with an average of: ";

  if (sec >= 0s) {
    std::cout << sec << " ";
  }

  std::cout << milli << " " << micro << " " << nano << std::endl;
  return 0;
}

auto chunk_test() -> void {
  using namespace goos;
  using namespace vm::op;
  vm::Chunk chunk{L"Test Chunk"};

  const struct {
    u16 funny = chunk.define_constant(420);
    u16 one = chunk.define_constant(1);
    u16 ten = chunk.define_constant(10);
    u16 zero = chunk.define_constant(0);
  } constants{};

  fmt::println("");

  #define line(...) chunk.write_instruction(Code::__VA_ARGS__);

  line(CONSTANT, constants.ten);

  const usize LOOP_BEGIN = chunk.label();

  // line(PRINT);

  line(CONSTANT, constants.one);

  line(PRINT);

  line(NEGATE);
  line(PRINT);

  line(ADD);

  line(DUP);

  // // 0 >= i
  line(CONSTANT, constants.zero);
  line(GREATER_OR_EQUAL_THAN);
  //
  line(JUMP_IF_FALSE, LOOP_BEGIN);

  // line(CONSTANT, constants.funny)
  line(PRINT);

  #undef line

  chunk.dissassemble();

  vm::Vm vm{std::move(chunk)};
  const vm::Value val = vm.run().take_unchecked();

  std::cout << "Exited with value " << val.type_name() << std::endl;
}
