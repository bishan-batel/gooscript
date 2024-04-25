//
// Created by bishan_ on 4/23/24.
//

#pragma once
#include <rc.hpp>
#include <box.hpp>
#include <token/Token.hpp>

namespace goos::lexer {
  class Lexer {
    using Token = token::Token;
    usize position;
    Rc<String> content;
    Vec<Box<Token> > tokens;

    explicit Lexer(const Rc<String> &content);

    void push(Box<Token> token) {
      tokens.push_back(std::move(token));
    }

    template<typename T, typename... Args> requires std::is_constructible_v<T, Args...>
    void emplace(Args... args) {
      push(crab::make_box<T>(args...));
    }

  public:
    static Vec<Box<Token> > tokenize(Rc<String> &content);

  private:
    [[nodiscard]] bool whitespace();

    [[nodiscard]] bool number_literal();

    [[nodiscard]] bool string_literal();

    [[nodiscard]] bool operator_tok();

    [[nodiscard]] bool identifier();
  };
}
