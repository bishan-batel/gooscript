//
// Created by bishan_ on 5/8/24.
//

#include "Text.hpp"

namespace goos::json {
  Text::Text(WideString text) : text{std::move(text)} {}

  auto Text::set_text(WideString value) -> void {
    text = std::move(value);
  }

  auto Text::get_text() const -> const WideString& {
    return text;
  }

  auto Text::write(std::wostream &os) const -> void {
    os << '"';
    escape(os, text);
    os << '"';
  }

  auto Text::clone() const -> Box<Value> {
    return crab::make_box<Text>(text);
  }
}
