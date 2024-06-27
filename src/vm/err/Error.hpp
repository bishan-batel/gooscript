//
// Created by bishan_ on 6/25/24.
//

#pragma once
#include <box.hpp>
#include <iomanip>
#include <result.hpp>
#include <sstream>

namespace goos::vm {
  namespace err {
    struct Error : crab::Error {};

    class UnsupportedOperation final : public Error {
      std::vector<StringView> operand_types;
      StringView operation;

    public:
      UnsupportedOperation(const StringView operation, std::vector<StringView> operand_types)
        : operand_types{std::move(operand_types)},
          operation{operation} {}

      [[nodiscard]] auto what() const -> String override {
        StringStream stream{};
        stream << "Unsupported operation " << std::quoted(operation) << " for values: [";
        for (const auto val: operand_types) {
          stream << val << ", ";
        }
        stream << "]";

        return stream.str();
      }
    };
  }

  template<typename T>
  using Result = ::Result<T, Box<err::Error>>;

  template<crab::result::error_type E, typename... Args> requires std::derived_from<E, err::Error>
  auto make_err(Args &&... args) -> Box<err::Error> {
    return Box<err::Error>{crab::make_box<E>(std::forward<Args>(args)...)};
  }
}
