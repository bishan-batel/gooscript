#pragma once

#include <option.hpp>
#include <unordered_map>

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

  inline static const std::unordered_map<std::string_view, Keyword> KEYWORD_TABLE{
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

  [[nodiscard]] Option<Keyword> identifier_to_keyword(std::string_view name);
} // namespace lexer
