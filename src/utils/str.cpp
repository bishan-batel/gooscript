//
// Created by bishan_ on 5/7/24.
//

#include "str.hpp"
#include <codecvt>
#include <locale>
#include <sstream>
#include <range.hpp>

namespace goos::str {
  auto repeat(const widechar character, const usize amount) -> WideString {
    WideStringStream stream{};

    for ([[maybe_unused]] const auto _: crab::range(amount)) {
      stream << character;
    }

    return stream.str();
  }

  auto left_pad(const WideStringView string, const WideStringView padding) -> WideString {
    WideStringStream stream{};

    // const WideString padding{repeat(pad_character, amount)};

    const auto add_padding{[&] { stream << padding; }};

    add_padding();

    for (const widechar c: string) {
      stream << c;
      if (c == '\n') {
        add_padding();
      }
    }

    return stream.str();
  }

  auto left_pad(const WideStringView string, const widechar pad_character, const usize count) -> WideString {
    return left_pad(string, repeat(pad_character, count));
  }

  auto convert(const WideStringView string) -> String {
    return std::wstring_convert<std::codecvt_utf8<widechar>>().to_bytes(string.data());
  }

  auto convert(const StringView string) -> WideString {
    return std::wstring_convert<std::codecvt_utf8<widechar>>().from_bytes(string.data());
  }
}
