#pragma once

#include <box.hpp>
#include <preamble.hpp>
#include "Request.hpp"
#include "json/Object.hpp"

namespace goos::lsp {
  [[nodiscard]] auto handle_message(Request request, Box<json::Object> content) -> bool;
}
