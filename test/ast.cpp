#include <catch2/catch_test_macros.hpp>

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
  SECTION("ScopeBlock") { TEST_CLONE(L"block {  }", ScopeBlock, {}) }
  SECTION("While") { TEST_CLONE(L"while unit { unit }", While, expr, expr) }
  SECTION("Binary") { TEST_CLONE(L"(unit) and (unit)", Binary, expr, goos::lexer::Operator::AND, expr); }
  SECTION("Unary") { TEST_CLONE(L"not (unit)", Unary, goos::lexer::Operator::NOT, expr); }
  SECTION("FunctionCall") { TEST_CLONE(L"funcall(unit)()", FunctionCall, expr, {}); }
  SECTION("IdentifierBinding") { TEST_CLONE(L"wha", IdentifierBinding, L"wha"); }
  SECTION("Eval") { TEST_CLONE(L"eval (unit)", Eval, expr) }
  SECTION("Return") { TEST_CLONE(L"return unit", Return, expr) }
  SECTION("Nil") { TEST_CLONE(L"nil", Nil) }
}
