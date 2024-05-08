#pragma once

#include <option.hpp>

namespace goos::lexer {
  enum class Keyword {
    MODULE,
    USE,
    LET,
    VAR,
    IF,
    ELSE,
    THEN,
    UNLESS,
    WHILE,
    UNTIL,
    FOR,
    DO,
    IN,
    NIL,
    UNIT,
    MATCH,
    DEFAULT,
    FN,
    RETURN,
    EVAL
  };

  inline static const Dictionary<WideStringView, Keyword> STR_TO_KEYWORD_TABLE{
    {L"module", Keyword::MODULE},
    {L"use", Keyword::USE},

    {L"let", Keyword::LET},
    {L"const", Keyword::LET},

    {L"var", Keyword::VAR},

    {L"if", Keyword::IF},
    {L"else", Keyword::ELSE},
    {L"then", Keyword::THEN},
    {L"unless", Keyword::UNLESS},
    {L"while", Keyword::WHILE},
    {L"break", Keyword::WHILE},
    {L"continue", Keyword::WHILE},
    {L"until", Keyword::UNTIL},
    {L"for", Keyword::FOR},
    {L"do", Keyword::DO},
    {L"in", Keyword::IN},
    {L"unit", Keyword::UNIT},
    {L"nil", Keyword::NIL},

    {L"null", Keyword::NIL},

    {L"match", Keyword::MATCH},
    {L"default", Keyword::DEFAULT},
    {L"fn", Keyword::FN},
    {L"return", Keyword::RETURN},
    {L"eval", Keyword::EVAL},
  };

  static const auto KEYWORD_TO_STR_MAP{
    [] {
      Dictionary<Keyword, WideStringView> map;
      for (const auto &[k, v]: STR_TO_KEYWORD_TABLE) {
        map.emplace(v, k);
      }
      return map;
    }()
  };

  [[nodiscard]] auto identifier_to_keyword(WideStringView name) -> Option<Keyword>;
} // namespace lexer
