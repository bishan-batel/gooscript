#include <array>
#include <catch2/catch_test_macros.hpp>
#include <utility>

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
#include "ast/expression/Binary.hpp"
#include "ast/expression/FunctionCall.hpp"
#include "ast/expression/IdentifierBinding.hpp"
#include "ast/expression/Unary.hpp"
#include "ast/statements/Eval.hpp"
#include "ast/statements/Return.hpp"
#include "ast/statements/VariableDeclaration.hpp"
#include "parser/TokenStream.hpp"
#include "parser/pass/statement/block.hpp"

using namespace goos;
using namespace goos::ast;

TEST_CASE("AST Into String", "[ast_string]") {
  using namespace goos::ast;
  using namespace goos::ast::expression;

  const Unit u{};

  #define TEST_CLONE(s, x, ...) REQUIRE(s == x{__VA_ARGS__}.clone()->to_string());
  #define expr u.clone_expr()

  SECTION("Unit") { TEST_CLONE(L"unit", Unit) }
  SECTION("Array") { TEST_CLONE(L"[]", Array, {}) }
  SECTION("Boolean") { TEST_CLONE(L"false", Boolean, false) }
  SECTION("Decimal") { TEST_CLONE(L"32D", Decimal, 32.0) }
  SECTION("Integer") { TEST_CLONE(L"420", Integer, 420) }
  SECTION("Dictionary") { TEST_CLONE(L"{}", expression::Dictionary, {}) }
  SECTION("StringLiteral") { TEST_CLONE(LR"("what")", expression::StringLiteral, L"what") }
  SECTION("Lambda") { TEST_CLONE(L"fn () = unit", Lambda, {}, u.clone_expr()) }
  SECTION("If") { TEST_CLONE(L"if unit then { unit } else { unit }", If, expr, expr, expr) }
  SECTION("ScopeBlock") {
    TEST_CLONE(L"block { unit; }", ScopeBlock, {})
  }
  SECTION("While") { TEST_CLONE(L"while unit { unit }", While, expr, expr) }
  SECTION("Binary") { TEST_CLONE(L"(unit) and (unit)", Binary, expr, goos::lexer::Operator::AND, expr); }
  SECTION("Unary") { TEST_CLONE(L"not (unit)", Unary, goos::lexer::Operator::NOT, expr); }
  SECTION("FunctionCall") { TEST_CLONE(L"funcall(unit)()", FunctionCall, expr, {}); }
  SECTION("IdentifierBinding") { TEST_CLONE(L"wha", IdentifierBinding, meta::Identifier::from(L"wha")); }
  SECTION("Eval") { TEST_CLONE(L"eval (unit)", Eval, expr) }
  SECTION("Return") { TEST_CLONE(L"return unit", Return, expr) }
  SECTION("Nil") { TEST_CLONE(L"nil", Nil) }

  #undef expr
  #undef TEST_CLONE
}

template<usize N>
auto parse(WideString source, std::array<Box<Statement>, N> statements) {
  Vec<Box<Statement>> vec{};

  for (auto &a: statements) {
    vec.push_back(std::move(a));
  }

  const expression::ScopeBlock expr{std::move(vec)};

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

  return Box<Statement>::wrap_unchecked(new expression::ScopeBlock{std::move(vec)});
}

#define UNIT crab::make_box<expression::Unit>()
#define DECL_VARIABLE(name, mut, expr) Box<Statement>::wrap_unchecked(new VariableDeclaration {goos::meta::Identifier::from(name), meta::Mutability:: mut, expr})

#define SCOPE(...) scope(std::array{__VA_ARGS__})

TEST_CASE("variables") {
  parse(
    L"let a = 0;",
    std::array{
      DECL_VARIABLE(L"a", IMMUTABLE, UNIT)
    }
  );

  parse(
    L"let a; let b; let c;",
    std::array{
      DECL_VARIABLE(L"a", IMMUTABLE, UNIT),
      DECL_VARIABLE(L"b", IMMUTABLE, UNIT),
      DECL_VARIABLE(L"c", IMMUTABLE, UNIT)
    }
  );

  REQUIRE_THROWS(parse( L"let ", std::array{ DECL_VARIABLE(L"a", IMMUTABLE, UNIT) } ));

  parse(
    L" let a; do { let b; let c; };",
    std::array{
      DECL_VARIABLE(L"a", IMMUTABLE, UNIT),
      SCOPE(
        DECL_VARIABLE(L"b", IMMUTABLE, UNIT),
        DECL_VARIABLE(L"c", IMMUTABLE, UNIT)
      )
    }
  );

  parse(
    L" let a; do { let b; let c; };",
    std::array{
      DECL_VARIABLE(L"a", IMMUTABLE, UNIT),
      SCOPE(
        DECL_VARIABLE(L"b", IMMUTABLE, UNIT),
        DECL_VARIABLE(L"c", IMMUTABLE, UNIT)
      )
    }
  );

  parse(
    L"do {  };",
    std::array{scope<0>({})}
  );

  parse(
    L"do { do{}; };",
    std::array{scope<1>({scope<0>({})})}
  );
}
