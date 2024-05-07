//
// Created by bishan_ on 5/7/24.
//

#include "str.hpp"

#include <codecvt>
#include <locale>

namespace goos::str {
  auto convert(const WideStringView string) -> String {
    return std::wstring_convert<std::codecvt_utf8<widechar>>().to_bytes(string.data());
  }

  auto convert(const StringView string) -> WideString {
    return std::wstring_convert<std::codecvt_utf8<widechar>>().from_bytes(string.data());
  }
}
