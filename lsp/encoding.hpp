#include <box.hpp>
#include <cstddef>
#include <fmt/xchar.h>
#include <range.hpp>
#include "Error.hpp"
#include "Marshal.hpp"

namespace goos::lsp::rpc {
  constexpr StringView HEADER = "Content-Length: ";
  constexpr StringView SEPERATOR = "\r\n\r\n";

  template<json_marshallable T>
  [[nodiscard]] auto encode(T msg) -> WideString {
    const auto content = JsonMarshal<T>::marshal(msg);

    return fmt::format(L"{} {}{}{}", HEADER, content.length(), SEPERATOR, content);
  }

  [[nodiscard]] auto decode(const Span<std::byte> bytes) -> Result<i32>;

} // namespace goos::lsp::rpc
