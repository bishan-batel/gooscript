#include <catch2/catch_test_macros.hpp>
#include <lexer/Keyword.hpp>

#include <lexer/Lexer.hpp>
#include <token/Keyword.hpp>

#include "token/Decimal.hpp"
#include "token/Identifier.hpp"
#include "token/Integer.hpp"
#include "token/StringLiteral.hpp"
#include "utils/str.hpp"

using namespace goos;
using namespace crab;

#define WORD(word) crab::make_box<goos::token::Keyword>(lexer::Keyword:: word)
#define IDENT(name) crab::make_box<goos::token::Identifier>(str::convert(#name))
#define NUMF(num) crab::make_box<goos::token::Decimal>(static_cast<f64>(num))
#define NUMI(num) crab::make_box<goos::token::Integer>(static_cast<i64>(num))
#define STR(str) crab::make_box<goos::token::StringLiteral>(str)

auto parse(lexer::TokenList &list, const WideStringView source) {
  list = std::move(lexer::Lexer::tokenize(SourceFile::create(WideString{source})).get_unchecked());
}

template<typename... Args>
auto matches(const lexer::TokenList &toks, Args... expected) {
  const auto size = sizeof...(expected);
  REQUIRE(toks.size() == size);

  lexer::TokenList list;

  (list.push_back(std::move(expected)), ...);

  for (usize i = 0; i < std::min(toks.size(), size); i++) {
    REQUIRE(*toks[i] == *list[i]);
  }
}

TEST_CASE("Lexer", "[lexer]") {
  using namespace token;

  lexer::TokenList toks;

  SECTION("Whitespace") {
    REQUIRE_NOTHROW(parse(toks, L"   \n \t \r\r\r \n \t "));
    REQUIRE(toks.empty());
  }

  SECTION("Identifier") {
    REQUIRE_NOTHROW(parse(toks, L" me\t\t when \n\rthe bruh\n huh "));

    matches(
      toks,
      IDENT(me),
      IDENT(when),
      IDENT(the),
      IDENT(bruh),
      IDENT(huh)
    );
  }

  SECTION("Keyword") {
    REQUIRE_NOTHROW(parse(toks, L" me let match var VAR what huh"));

    matches(
      toks,
      IDENT(me),
      WORD(LET),
      WORD(MATCH),
      WORD(VAR),
      IDENT(VAR),
      IDENT(what),
      IDENT(huh)
    );

    REQUIRE_NOTHROW(
      parse(
        toks,
        L"module "
        L"use "
        L"let "
        L"var "
        L"if "
        L"else "
        L"then "
        L"unless "
        L"while "
        L"until "
        L"for "
        L"do "
        L"in "
        L"nil "
        L"match "
        L"default "
        L"fn "
        L"return "
        L"unit"
      )
    );

    matches(
      toks,
      WORD(MODULE),
      WORD(USE),
      WORD(LET),
      WORD(VAR),
      WORD(IF),
      WORD(ELSE),
      WORD(THEN),
      WORD(UNLESS),
      WORD(WHILE),
      WORD(UNTIL),
      WORD(FOR),
      WORD(DO),
      WORD(IN),
      WORD(NIL),
      WORD(MATCH),
      WORD(DEFAULT),
      WORD(FN),
      WORD(RETURN),
      WORD(UNIT)
    );
  }

  SECTION("Number") {
    REQUIRE_NOTHROW(parse(toks, LR"(42 42. 42.02what)"));

    matches(
      toks,
      NUMI(42),
      NUMF(42.),
      NUMF(42.02),
      IDENT(what)
    );

    REQUIRE_NOTHROW(parse(toks, LR"(huh let42 let 42. 42.02what)"));

    matches(
      toks,
      IDENT(huh),
      IDENT(let42),
      WORD(LET),
      NUMF(42),
      NUMF(42.02),
      IDENT(what)
    );
  }

  SECTION("String") {
    SECTION("Normal") {
      REQUIRE_NOTHROW(
        parse(
          toks,
          LR"( "huh" "me when the"42 )")
      );

      matches(
        toks,
        STR(L"huh"),
        STR(L"me when the"),
        NUMI(42)
      );
    }

    SECTION("Unterminated String") {
      REQUIRE_THROWS(
        parse(
          toks,
          LR"( "huh" "me when the )")
      );

      REQUIRE_THROWS(
        parse(
          toks,
          LR"( huh""what" )")
      );

      REQUIRE_NOTHROW(
        parse(
          toks,
          LR"( huh"""what" )")
      );

      matches(
        toks,
        IDENT(huh),
        STR(L""),
        STR(L"what")
      );
    }
  }
}
