//
// Created by bishan_ on 6/14/24.
//

#pragma once
#include "parser/Error.hpp"

namespace goos::runtime::err {
  class Error : public crab::Error {};

  template<typename T, typename... Args> requires std::derived_from<T, Error> and std::is_constructible_v<T, Args...>
  auto make(Args &&... args) -> crab::result::Err<Box<Error>> {
    return crab::err<Box<Error>>(crab::make_box<T>(args...));
  }
}
