//
// Created by bishan_ on 4/23/24.
//

#pragma once
#include <rc.hpp>

namespace goos::lexer {
  class Tokenizer {
  public:
    explicit Tokenizer(const Rc<String> &content);

  private:
    Rc<String> content;
  };
}
