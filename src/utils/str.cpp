//
// Created by bishan_ on 5/7/24.
//

#include "str.hpp"

#include <codecvt>
#include <locale>

namespace goos::str {
  String convert(WideStringView string) {
    return std::wstring_convert<std::codecvt_utf8<widechar>>().to_bytes(string.data());
  }

  WideString convert(const StringView string) {
    return std::wstring_convert<std::codecvt_utf8<widechar>>().from_bytes(string.data());
  }
}
