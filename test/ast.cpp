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

  SECTION("Unit") { TEST_CLONE("unit", Unit) }
  SECTION("Array") { TEST_CLONE("[]", Array, {}) }
  SECTION("Boolean") { TEST_CLONE("false", Boolean, false) }
  SECTION("Decimal") { TEST_CLONE("32D", Decimal, 32.0) }
  SECTION("Integer") { TEST_CLONE("420", Integer, 420) }
  SECTION("Dictionary") { TEST_CLONE("{}", expression::Dictionary, {}) }
  SECTION("StringLiteral") { TEST_CLONE(R"("what")", expression::StringLiteral, "what") }
  SECTION("Lambda") { TEST_CLONE("fn () = unit", Lambda, {}, u.clone_expr()) }
  SECTION("If") { TEST_CLONE("if unit then { unit } else { unit }", If, expr, expr, expr) }
  SECTION("ScopeBlock") { TEST_CLONE("scope {  }", ScopeBlock, {}) }
  SECTION("While") { TEST_CLONE("while unit { unit }", While, expr, expr) }
  SECTION("Binary") { TEST_CLONE("(unit) and (unit)", Binary, expr, goos::lexer::Operator::AND, expr); }
  SECTION("Unary") { TEST_CLONE("not (unit)", Unary, goos::lexer::Operator::NOT, expr); }
  SECTION("FunctionCall") { TEST_CLONE("funcall(unit)()", FunctionCall, expr, {}); }
  SECTION("IdentifierBinding") { TEST_CLONE("wha", IdentifierBinding, "wha"); }
  SECTION("Eval") { TEST_CLONE("eval (unit)", Eval, expr) }
  SECTION("Return") { TEST_CLONE("return unit", Return, expr) }
  SECTION("Nil") { TEST_CLONE("nil", Nil) }
}
