//
// Created by bishan_ on 5/3/24.
//

#pragma once
#include <range.hpp>
#include <rc.hpp>
#include <result.hpp>
#include <token/Token.hpp>

namespace goos::parser::err {
  class Error : public crab::Error {
    Box<token::Token> token;
    String msg;

  public:
    explicit Error(const token::Token &token);
  };
}
