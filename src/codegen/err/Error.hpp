//
// Created by bishan_ on 6/27/24.
//

#pragma once
#include <box.hpp>
#include <result.hpp>

namespace goos::codegen {
  namespace err {
    struct Error : crab::Error {};
  }

  template<crab::result::ok_type T>
  using CompilerResult = Result<T, Box<err::Error>>;
}
