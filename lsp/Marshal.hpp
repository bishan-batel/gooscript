#include <preamble.hpp>

namespace goos::lsp {

  template<typename T>
  struct JsonMarshal {
    static auto marshal(const T &val) -> WideString { return static_cast<WideString>(JsonMarshal<T>{}(val)); }
  };

  template<typename T>
  concept json_marshallable = requires(JsonMarshal<T> m, const T &v) {
    { m(v) } -> std::convertible_to<WideString>;
  };

} // namespace goos::lsp
