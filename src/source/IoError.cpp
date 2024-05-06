//
// Created by bishan_ on 5/5/24.
//

#include "IoError.hpp"
#include <fmt/format.h>

String goos::io::Error::what() const {
  return fmt::format("Failed to Open File: {}", std::strerror(error_code));
}

i32 goos::io::Error::get_system_error_code() const {
  return error_code;
}

goos::io::Error::Error(const i32 error_code) : error_code{error_code} {}
