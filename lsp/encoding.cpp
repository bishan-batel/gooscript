#include "encoding.hpp"
#include <sstream>

namespace goos::lsp::rpc {
  auto decode(const Span<std::byte> bytes) -> Result<i32> {
    if (bytes.size() < HEADER.length() + SEPERATOR.size() + 1) {
      return err::make<err::MalformedDecode>(err::MalformedDecode::Type::FAILED_TO_FIND_SEPERATOR);
    }

    Option<usize> seperator_index = crab::none;

    for (usize i = HEADER.length(); i < bytes.size() - SEPERATOR.size(); i++) {
      bool found = true;

      for (usize j = 0; j < SEPERATOR.size(); j++) {
        if ((bytes[i]) != static_cast<std::byte>(SEPERATOR[j])) {
          found = false;
          break;
        }
      }

      if (not found)
        continue;

      seperator_index = i;

      break;
    }

    if (seperator_index.is_none()) {
      return err::make<err::MalformedDecode>(err::MalformedDecode::Type::FAILED_TO_FIND_SEPERATOR);
    }

    String content_length_str{};

    for (auto i = content_length_str.size(); i < seperator_index.get_unchecked(); i++) {
      content_length_str.push_back(static_cast<char>(bytes[i]));
    }

    usize content_length = 0;
    std::istringstream int_stream{content_length_str};
    int_stream >> content_length;


    if (int_stream.fail()) {
      return err::make<err::MalformedDecode>(err::MalformedDecode::Type::INVALID_CONTENT_LENGTH);
    }

    [[maybe_unused]] const Span<std::byte> content = bytes.subspan(seperator_index + SEPERATOR.size());

    return 0;
  }

} // namespace goos::lsp::rpc
