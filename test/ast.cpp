#include <array>
#include <catch2/catch_test_macros.hpp>
#include <rc.hpp>
#include <utility>

#include "ast/Statement.hpp"
#include "ast/expression/Binary.hpp"
#include "ast/expression/FunctionCall.hpp"
#include "ast/expression/IdentifierBinding.hpp"
#include "ast/expression/Unary.hpp"
#include "ast/expression/compound/If.hpp"
#include "ast/expression/compound/ScopeBlock.hpp"
#include "ast/expression/compound/While.hpp"
#include "ast/expression/literal/Array.hpp"
#include "ast/expression/literal/Boolean.hpp"
#include "ast/expression/literal/Decimal.hpp"
#include "ast/expression/literal/Dictionary.hpp"
#include "ast/expression/literal/Integer.hpp"
#include "ast/expression/literal/Lambda.hpp"
#include "ast/expression/literal/Nil.hpp"
#include "ast/expression/literal/StringLiteral.hpp"
#include "ast/expression/literal/Unit.hpp"
#include "ast/statements/Eval.hpp"
#include "ast/statements/Return.hpp"
#include "ast/statements/VariableDeclaration.hpp"
#include "parser/TokenStream.hpp"
#include "parser/pass/statement/block.hpp"
#include "source/SourceFile.hpp"
#include "token/Integer.hpp"

using namespace goos;
using namespace goos::ast;

static const SourceFile DUMMY_FILE = SourceFile::create("dummy_file", L"huh");
static const TokenTrace DUMMY_TRACE{crab::make_rc<token::Integer>(DUMMY_FILE, crab::range<usize>(0, 1), 0)};

TEST_CASE("AST Into String", "[ast_string]") {
  using namespace goos::ast;
  using namespace goos::ast::expression;

  const Unit u{DUMMY_TRACE};

#define TEST_CLONE(s, x, ...) REQUIRE(s == x{__VA_ARGS__}.clone()->to_string())
#define expr u.clone_expr()

  SECTION("Unit") { TEST_CLONE(L"unit", Unit, DUMMY_TRACE); }
  SECTION("Array") { TEST_CLONE(L"[]", Array, {}, DUMMY_TRACE); }
  SECTION("Boolean") { TEST_CLONE(L"false", Boolean, false, DUMMY_TRACE); }
  SECTION("Decimal") { TEST_CLONE(L"32D", Decimal, 32.0, DUMMY_TRACE); }
  SECTION("Integer") { TEST_CLONE(L"420", Integer, 420, DUMMY_TRACE); }
  SECTION("Dictionary") { TEST_CLONE(L"{}", expression::Dictionary, {}, DUMMY_TRACE); }
  SECTION("StringLiteral") {
    TEST_CLONE(LR"("what")", expression::StringLiteral, crab::make_rc<WideString>(L"what"), DUMMY_TRACE);
  }
  SECTION("Lambda") { TEST_CLONE(L"fn () = unit", Lambda, {}, u.clone_expr(), DUMMY_TRACE); }
  SECTION("If") { TEST_CLONE(L"if unit then { unit } else { unit }", If, expr, expr, expr, DUMMY_TRACE); }
  SECTION("ScopeBlock") { TEST_CLONE(L"block { unit; }", ScopeBlock, {}, DUMMY_TRACE); }
  SECTION("While") { TEST_CLONE(L"while unit { unit }", While, expr, expr, DUMMY_TRACE); }
  SECTION("Binary") { TEST_CLONE(L"(unit) and (unit)", Binary, expr, goos::lexer::Operator::AND, expr, DUMMY_TRACE); }
  SECTION("Unary") { TEST_CLONE(L"not (unit)", Unary, goos::lexer::Operator::NOT, expr, DUMMY_TRACE); }
  SECTION("FunctionCall") { TEST_CLONE(L"funcall(unit)()", FunctionCall, expr, {}, DUMMY_TRACE); }
  SECTION("IdentifierBinding") { TEST_CLONE(L"wha", IdentifierBinding, meta::Identifier::from(L"wha"), DUMMY_TRACE); }
  SECTION("Eval") { TEST_CLONE(L"eval (unit)", Eval, expr, DUMMY_TRACE); }
  SECTION("Return") { TEST_CLONE(L"return unit", Return, expr, DUMMY_TRACE); }
  SECTION("Nil") { TEST_CLONE(L"nil", Nil, DUMMY_TRACE); }

#undef expr
#undef TEST_CLONE
}

template<usize N>
auto parse(WideString source, std::array<Box<Statement>, N> statements) {
  Vec<Box<Statement>> vec{};

  for (auto &a: statements) {
    vec.push_back(std::move(a));
  }

  const expression::ScopeBlock expr{std::move(vec), DUMMY_TRACE};

  const SourceFile file = SourceFile::create(std::move(source));
  parser::TokenStream stream{lexer::Lexer::tokenize(file).take_unchecked()};

  Box<expression::ScopeBlock> block = parser::pass::block_top_level(stream).take_unchecked();
  REQUIRE(expr == *block);
}

template<usize N>
auto scope(std::array<Box<Statement>, N> statements) {
  Vec<Box<Statement>> vec{};

  for (auto &a: statements) {
    vec.push_back(std::move(a));
  }

  return Box<Statement>::wrap_unchecked(new expression::ScopeBlock{std::move(vec), DUMMY_TRACE});
}

#define UNIT crab::make_box<expression::Unit>(DUMMY_TRACE)
#define DECL_VARIABLE(name, mut, expr)                                                                                 \
  Box<Statement>::wrap_unchecked(                                                                                      \
      new VariableDeclaration{goos::meta::Identifier::from(name), meta::Mutability::mut, expr, DUMMY_TRACE})

#define SCOPE(...) scope(std::array{__VA_ARGS__})

TEST_CASE("variables") {
  parse(L"let a = 0;", std::array{DECL_VARIABLE(L"a", IMMUTABLE, UNIT)});

  parse(
      L"let a; let b; let c;",
      std::array{
          DECL_VARIABLE(L"a", IMMUTABLE, UNIT),
          DECL_VARIABLE(L"b", IMMUTABLE, UNIT),
          DECL_VARIABLE(L"c", IMMUTABLE, UNIT)});

  REQUIRE_THROWS(parse(L"let ", std::array{DECL_VARIABLE(L"a", IMMUTABLE, UNIT)}));

  parse(
      L" let a; do { let b; let c; };",
      std::array{
          DECL_VARIABLE(L"a", IMMUTABLE, UNIT),
          SCOPE(DECL_VARIABLE(L"b", IMMUTABLE, UNIT), DECL_VARIABLE(L"c", IMMUTABLE, UNIT))});

  parse(
      L" let a; do { let b; let c; };",
      std::array{
          DECL_VARIABLE(L"a", IMMUTABLE, UNIT),
          SCOPE(DECL_VARIABLE(L"b", IMMUTABLE, UNIT), DECL_VARIABLE(L"c", IMMUTABLE, UNIT))});

  parse(L"do {  };", std::array{scope<0>({})});

  parse(L"do { do{}; };", std::array{scope<1>({scope<0>({})})});
}
