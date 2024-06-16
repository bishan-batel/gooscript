//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include "parser/Error.hpp"
#include "runtime/data/IValue.hpp"

namespace goos::runtime {
  namespace err {
    class Error : public crab::Error {};

    template<typename T, typename... Args> requires std::derived_from<T, Error> and std::is_constructible_v<T, Args...>
    auto make(Args &&... args) -> crab::result::Err<Box<Error>> {
      return crab::err<Box<Error>>(crab::make_box<T>(args...));
    }
  }

  template<typename T>
  using Result = Result<T, Box<err::Error>>;

  using VoidResult = Option<Box<err::Error>>;

  template<typename T> requires std::is_base_of_v<IValue, T>
  auto ok(RcMut<T> any) -> Result<Any> {
    return Result<Any>{any};
  }
}

namespace goos::runtime {}
