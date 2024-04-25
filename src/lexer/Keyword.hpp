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
    MATCH,
    DEFAULT,
    FN,
    RETURN,
  };

  inline static const Dictionary<StringView, Keyword> STR_TO_KEYWORD_TABLE{
    {"module", Keyword::MODULE},
    {"use", Keyword::USE},

    {"let", Keyword::LET},
    {"const", Keyword::LET},

    {"var", Keyword::VAR},

    {"if", Keyword::IF},
    {"else", Keyword::ELSE},
    {"then", Keyword::THEN},
    {"unless", Keyword::UNLESS},
    {"while", Keyword::WHILE},
    {"break", Keyword::WHILE},
    {"continue", Keyword::WHILE},
    {"until", Keyword::UNTIL},
    {"for", Keyword::FOR},
    {"do", Keyword::DO},
    {"in", Keyword::IN},
    {"nil", Keyword::NIL},

    {"null", Keyword::NIL},

    {"match", Keyword::MATCH},
    {"default", Keyword::DEFAULT},
    {"fn", Keyword::FN},
    {"return", Keyword::RETURN},
  };

  static const auto KEYWORD_TO_STR_MAP{
    [] {
      Dictionary<Keyword, StringView> map;
      for (const auto &[k, v]: STR_TO_KEYWORD_TABLE) {
        map.emplace(v, k);
      }
      return map;
    }()
  };

  [[nodiscard]] Option<Keyword> identifier_to_keyword(std::string_view name);
} // namespace lexer
