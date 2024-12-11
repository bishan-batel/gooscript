#pragma once

#include <algorithm>
#include <bits/ranges_algo.h>
#include <map>
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
    SUB = binary | unary,
    MUL = binary,
    DIV = binary,
    MOD = binary,
    ARROW = delimiter,
    THIN_ARROW = delimiter,
    COLON = delimiter,
    DOT = delimiter | binary,
    SEMICOLON = delimiter,
    COMMA = delimiter,
    PAREN_OPEN = paren(OPEN),
    PAREN_CLOSE = paren(CLOSE),
    BRACKET_OPEN = paren(OPEN),
    BRACKET_CLOSE = paren(CLOSE),
    CURLY_OPEN = paren(OPEN),
    CURLY_CLOSE = paren(CLOSE),
    IN = binary
  };

  static const Vec<std::pair<WideStringView, Operator>> STR_TO_OPERATOR_MAP = [] {
    using Pair = std::pair<WideStringView, Operator>;
    Vec<Pair> map{
      {L"and", Operator::AND},
      {L"not", Operator::NOT},
      {L"mod", Operator::MOD},
      {L"or", Operator::OR},
      {L"in", Operator::IN},
      {L"-=", Operator::SUB_ASSIGN},
      {L"%=", Operator::MOD_ASSIGN},
      {L"*=", Operator::MUL_ASSIGN},
      {L"/=", Operator::DIV_ASSIGN},
      {L"+=", Operator::ADD_ASSIGN},
      {L"|=", Operator::BIT_OR_ASSIGN},
      {L"&=", Operator::BIT_AND_ASSIGN},
      {L"==", Operator::EQUALS},
      {L"!=", Operator::NOT_EQUALS},
      {L">=", Operator::GREATER_OR_EQUALS},
      {L"<=", Operator::LESS_OR_EQUALS},
      {L"<<", Operator::SHIFT_LEFT},
      {L">>", Operator::SHIFT_RIGHT},
      {L"=>", Operator::ARROW},
      {L"^", Operator::XOR},
      {L"|", Operator::BIT_OR},
      {L"&", Operator::BIT_AND},
      {L">", Operator::GREATER},
      {L"<", Operator::LESS},
      {L"+", Operator::ADD},
      {L"*", Operator::MUL},
      {L"/", Operator::DIV},
      {L"%", Operator::MOD},
      {L";", Operator::SEMICOLON},
      {L":", Operator::COLON},
      {L".", Operator::DOT},
      {L",", Operator::COMMA},
      {L"(", Operator::PAREN_OPEN},
      {L")", Operator::PAREN_CLOSE},
      {L"[", Operator::BRACKET_OPEN},
      {L"]", Operator::BRACKET_CLOSE},
      {L"{", Operator::CURLY_OPEN},
      {L"}", Operator::CURLY_CLOSE},
      {L"-", Operator::SUB},
      {L"=", Operator::ASSIGN},
    };

    std::ranges::sort(map, [](const Pair &lhs, const Pair &rhs) { return lhs.first.size() > rhs.first.size(); });

    return map;
  }();

  static const auto OPERATOR_TO_STR_MAP{[] {
    Dictionary<Operator, WideStringView> map;
    for (const auto &[k, v]: STR_TO_OPERATOR_MAP) {
      map.emplace(v, k);
    }
    return map;
  }()};

  [[maybe_unused]] constexpr auto is_assign(Operator op) -> bool { return static_cast<u32>(op) & ASSIGN_FLAG; }

  [[maybe_unused]] constexpr auto is_binary(Operator op) -> bool { return static_cast<u32>(op) & BINARY_FLAG; }

  [[nodiscard]] constexpr auto is_unary(Operator op) -> bool { return static_cast<u32>(op) & UNARY_FLAG; }

  [[maybe_unused]] constexpr auto is_delimiter(Operator op) -> bool { return static_cast<u32>(op) & DELIMITER_FLAG; }

  [[maybe_unused]] constexpr auto is_parenthetical(Operator op) -> bool {
    const u32 bits = static_cast<u32>(op);
    return bits & PARENTHETICAL_CLOSE_FLAG or bits & PARENTHETICAL_OPEN_FLAG;
  }

#undef binary
#undef assign
#undef compare
#undef unary
#undef delimiter
#undef paren
} // namespace goos::lexer
