//
// Created by bishan_ on 5/7/24.
//

#pragma once

#include <sstream>
#include <ranges>

#include "preamble.hpp"

namespace goos::str {
  /**
   * @brief Creates a string with the given character repeated 'amount' times
   * @param character The character to repeat
   * @param amount The number of times the character should be repeated
   */
  [[nodiscard]] auto repeat(widechar character, usize amount) -> WideString;

  /**
   * Left pads each line of the given string with the given string
   * @param string String to pad
   * @param padding String to be added at the beginning of each line
   */
  [[nodiscard]] auto left_pad(WideStringView string, WideStringView padding) -> WideString;

  /**
   * Left pads each line of the given string with the given character 'count' count of times
   * @param string
   * @param pad_character The character that is repeated at the beginning of each line
   * @param count Amount of pad_character at the beginning of each line
   */
  [[nodiscard]] auto left_pad(WideStringView string, widechar pad_character, usize count) -> WideString;

  /**
   * @brief Converts a wide string to a string
   */
  [[nodiscard]] auto convert(WideStringView string) -> String;

  template<std::ranges::input_range Range, typename F>
  [[nodiscard]] auto join(Range &&range, F to_string, WideStringView seperator = L", ") -> WideString;

  /**
   * @brief Converts a string to a wide string
   */
  [[nodiscard]] auto convert(StringView string) -> WideString;
}

namespace goos::str {
  template<std::ranges::input_range Range, typename F>
  auto join(Range &&range, F to_string, const WideStringView seperator) -> WideString {
    {
      WideStringStream stream{};

      usize n = 0;
      for (const auto &element: range) {
        stream << to_string(element);

        stream << seperator;
        n++;
      }

      if (n > 0) {
        const WideString string{stream.str()};
        return string.substr(0, string.size() - seperator.length());
      }

      return stream.str();
    }
  }
}
