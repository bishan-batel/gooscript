//
// Created by bishan_ on 5/7/24.
//

#pragma once

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

  /**
   * @brief Converts a string to a wide string
   */
  [[nodiscard]] auto convert(StringView string) -> WideString;
}
