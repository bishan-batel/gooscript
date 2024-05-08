//
// Created by bishan_ on 5/8/24.
//

#include "Value.hpp"

namespace goos::json {
  auto operator<<(std::wostream &os, const Value &value) -> std::wostream& {
    value.write(os);
    return os;
  }

  auto escape(std::wostream &os, const WideStringView str) -> void {
    for (const widechar c: str) {
      #define CASE(x, y) case x: { os << y; continue; }
      switch (c) {
        CASE(L'\\', "\\\\");
        CASE(L'\n', "\\n");
        CASE(L'\t', "\\t");
        CASE(L'\r', "\\r");
        CASE(L'"', "\\\"");
        default: {
          os << c;
        }
      }
      #undef CASE
    }
  }
}
