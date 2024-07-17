#include <box.hpp>
#include <concepts>
#include <crab/debug.hpp>
#include <preamble.hpp>
#include <result.hpp>

namespace goos::lsp {
  namespace err {
    struct Error : crab::Error {};

    struct MalformedDecode final : Error {
      enum class Type { FAILED_TO_FIND_SEPERATOR, INVALID_CONTENT_LENGTH } type;

      explicit MalformedDecode(Type type) : type{type} {}

      [[nodiscard]] auto what() const -> String final {
        switch (type) {
          case Type::FAILED_TO_FIND_SEPERATOR:
            return "Failed to find seperator";
          case Type::INVALID_CONTENT_LENGTH:
            return "Invalid Content Length";

          default:
            debug_assert(false, "Invalid decode type");
            std::unreachable();
        }
      }
    };

    template<std::derived_from<err::Error> T, typename... Args>
    auto make(Args &&...args) -> crab::result::Err<Box<err::Error>>
      requires std::constructible_from<T, Args...>
    {
      return crab::err<Box<err::Error>>(crab::make_box<T>(std::forward<Args>(args)...));
    }

  } // namespace err
  template<typename T>
  using Result = ::Result<T, Box<err::Error>>;

} // namespace goos::lsp
