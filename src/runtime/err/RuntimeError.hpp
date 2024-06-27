//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include "parser/Error.hpp"
#include "../data/interfaces/IValue.hpp"
#include <any>

namespace goos::runtime {
  namespace err {
    class Error : public crab::Error {};

    template<typename T, typename... Args> requires std::derived_from<T, Error> and std::is_constructible_v<T, Args...>
    auto make(Args &&... args) -> crab::result::Err<Box<Error>> {
      return crab::err<Box<Error>>(crab::make_box<T>(args...));
    }
  }

  // template<typename T>
  // using Result = Result<T, Box<err::Error>>;

  // using VoidResult = Option<Box<err::Error>>;

  template<typename T> requires std::is_base_of_v<IValue, T>
  auto ok(RcMut<T> any) -> Result<std::any, Box<crab::Error>> {
    return {std::any{Any{any}}};
  }

  template<std::derived_from<err::Error> T>
  auto error(Box<T> any) -> Result<std::any, Box<crab::Error>> {
    return Box<crab::Error>{std::move(any)};
  }
}

namespace goos::runtime {}
