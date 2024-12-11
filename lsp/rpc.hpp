#pragma once

#include <box.hpp>
#include <fmt/xchar.h>
#include <iostream>
#include <ostream>
#include <preamble.hpp>
#include <range.hpp>
#include "Error.hpp"
#include "Marshal.hpp"
#include "json/Object.hpp"
#include "utils/str.hpp"

namespace goos::lsp::rpc {
  // auto log() -> std::wofstream &;

  constexpr StringView HEADER = "Content-Length: ";
  constexpr StringView SEPERATOR = "\r\n\r\n";

  using Id = std::variant<i64, String>;

  template<json::marshallable T>
  [[nodiscard]] auto encode(const T &msg) -> WideString {
    const Box<json::Value> content = json::marshal(msg);

    WideStringStream stream{};
    content->write(stream);

    const WideString str = stream.str();

    return fmt::format(L"{}{}{}{}", str::convert(HEADER), str.length(), str::convert(SEPERATOR), str);
  }

  template<json::marshallable T>
  auto send_result(const T &msg) -> Result<unit> {
    const WideString packet = encode(msg);
    std::wcout << packet << std::flush;
    return unit{};
  }

  [[nodiscard]] auto is_msg_ready(const Span<u8> bytes) -> Result<bool>;

  [[nodiscard]] auto decode(const Span<u8> bytes) -> Result<Box<json::Object>>;

} // namespace goos::lsp::rpc
