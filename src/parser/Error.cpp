//
// Created by bishan_ on 5/3/24.
//

#include "Error.hpp"

goos::parser::Error::Error(const token::Token &token) : token{token.clone()} {}
