#pragma once

#include <preamble.hpp>

namespace goos::lexer {
  enum [[maybe_unused]] OperatorFlag : u32 {
    COMPARISON_FLAG = 1 << 30,
    BINARY_FLAG = 1 << 29,
    ASSIGN_FLAG = 1 << 28,
    UNARY_FLAG = 1 << 27,
    DELIMITER_FLAG = 1 << 26,
    PARENTHETICAL = 1 << 25,
    PARENTHETICAL_OPEN_FLAG = 1 << 25,
    PARENTHETICAL_CLOSE_FLAG = 1 << 24,
  };

  #define binary (__COUNTER__ | BINARY_FLAG)
  #define assign (__COUNTER__ | BINARY_FLAG | ASSIGN_FLAG)

  #define compare (__COUNTER__ | BINARY_FLAG | COMPARISON_FLAG)
  #define unary (__COUNTER__ | UNARY_FLAG)
  #define delimiter (__COUNTER__ | DELIMITER_FLAG)

  #define paren(closed) (__COUNTER__ | PARENTHETICAL_##closed##_FLAG)

  enum class [[maybe_unused]] Operator : u32 {
    ASSIGN = assign,
    MOD_ASSIGN = assign,
    MUL_ASSIGN = assign,
    DIV_ASSIGN = assign,
    ADD_ASSIGN = assign,
    SUB_ASSIGN = assign,
    BIT_OR_ASSIGN = assign,
    BIT_AND_ASSIGN = assign,

    XOR = binary,
    BIT_OR = binary,
    BIT_AND = binary,
    AND = binary,
    OR = binary,
    NOT = unary,
    EQUALS = compare,
    NOT_EQUALS = compare,
    GREATER = compare,
    LESS = compare,
    GREATER_OR_EQUALS = compare,
    LESS_OR_EQUALS = compare,
    SHIFT_LEFT = binary,
    SHIFT_RIGHT = binary,
    ADD = binary,
    SUB = binary,
    MUL = binary,
    DIV = binary,
    MOD = binary,
    ARROW = delimiter,
    THIN_ARROW = delimiter,
    SEMICOLON = delimiter,
    COMMA = delimiter,

    PAREN_OPEN = paren(OPEN),
    PAREN_CLOSE = paren(CLOSE),

    BRACKET_OPEN = paren(OPEN),
    BRACKET_CLOSE = paren(CLOSE),

    CURLY_OPEN = paren(OPEN),
    CURLY_CLOSE = paren(CLOSE),
  };

  static const Dictionary<StringView, Operator> STR_TO_OPERATOR_MAP{
    {"=", Operator::ASSIGN},
    {"%=", Operator::MOD_ASSIGN},
    {"*=", Operator::MUL_ASSIGN},
    {"/=", Operator::DIV_ASSIGN},
    {"+=", Operator::ADD_ASSIGN},
    {"-=", Operator::SUB_ASSIGN},
    {"|=", Operator::BIT_OR_ASSIGN},
    {"&=", Operator::BIT_AND_ASSIGN},
    {"^", Operator::XOR},
    {"|", Operator::BIT_OR},
    {"&", Operator::BIT_AND},
    {"and", Operator::AND},
    {"or", Operator::OR},
    {"not", Operator::NOT},
    {"==", Operator::EQUALS},
    {"!=", Operator::NOT_EQUALS},
    {">", Operator::GREATER},
    {"<", Operator::LESS},
    {">=", Operator::GREATER_OR_EQUALS},
    {"<=", Operator::LESS_OR_EQUALS},
    {"<<", Operator::SHIFT_LEFT},
    {">>", Operator::SHIFT_RIGHT},
    {"+", Operator::ADD},
    {"-", Operator::SUB},
    {"*", Operator::MUL},
    {"/", Operator::DIV},
    {"%", Operator::MOD},
    {"=>", Operator::ARROW},
    {";", Operator::SEMICOLON},
    {":", Operator::COMMA},
    {"(", Operator::PAREN_OPEN},
    {")", Operator::PAREN_CLOSE},
    {"[", Operator::BRACKET_OPEN},
    {"]", Operator::BRACKET_CLOSE},
    {"{", Operator::CURLY_OPEN},
    {"}", Operator::CURLY_CLOSE},
  };

  static const auto OPERATOR_TO_STR_MAP{
    [] {
      Dictionary<Operator, StringView> map;
      for (const auto &[k, v]: STR_TO_OPERATOR_MAP) {
        map.emplace(v, k);
      }
      return map;
    }()
  };

  [[maybe_unused]]
  bool is_binary(Operator op);

  [[maybe_unused]]
  bool is_assign(Operator op);

  [[maybe_unused]]
  bool is_delimiter(Operator op);

  [[maybe_unused]]
  bool is_parenthetical(Operator op);

  #undef binary
  #undef assign
  #undef compare
  #undef unary
  #undef delimiter
  #undef paren
} // namespace lexer
