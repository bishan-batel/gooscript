#pragma once

#include <crab/debug.hpp>
#include <filesystem>
#include <fmt/ostream.h>
#include <fmt/xchar.h>
#include <fstream>
#include <preamble.hpp>

namespace goos::lsp::log {
  enum class Level { TRACE, INFO, ERROR, FATAL };

  constexpr auto to_string(Level level) -> StringView {
    switch (level) {
      case Level::TRACE:
        return "Trace";
      case Level::INFO:
        return "Info";
      case Level::ERROR:
        return "Error";
      case Level::FATAL:
        return "Fatal";
    }

    return "INVALID_LOG_LEVEL";
  }

  inline static std::wofstream LOG_FILE{};

  inline static Level TRACE_LEVEL =
#if DEBUG
    Level::TRACE;
#else
    Level::INFO;
#endif

  inline auto init_logfile(const std::filesystem::path file_name = "gooscript-lsp.log") -> void {
    LOG_FILE.open(file_name);
    debug_assert(LOG_FILE.good(), "Failed to open logfile");
  }

  template<typename... Args>
  auto print(Level, Args...) -> void {
    // if (level < TRACE_LEVEL) {
    //   return;
    // }
    //
    // fmt::print(LOG_FILE, args...);
  }

  template<typename... Args>
  auto trace(Args...) -> void {
    // print(Level::TRACE, args...);
  }

  template<typename... Args>
  auto info(Args...) -> void {
    // print(Level::INFO, args...);
  }

  template<typename... Args>
  auto error(Args...) -> void {
    // print(Level::ERROR, args...);
  }

  template<typename... Args>
  auto fatal(Args...) -> void {
    // print(Level::FATAL, args...);
  }

} // namespace goos::lsp::log
