//
// Created by bishan_ on 5/5/24.
//

#pragma once

#include <filesystem>
#include <range.hpp>
#include <rc.hpp>

#include "preamble.hpp"
#include "result.hpp"
#include "source/IoError.hpp"

namespace goos {
  class SourceFile {
    Rc<Option<String>> name;
    Rc<WideString> contents;

    SourceFile(String name, WideString contents);

    explicit SourceFile(WideString contents);

  public:
    /**
     * Creates a source file with given name and
     */
    static auto create(String name, WideString contents) -> SourceFile;

    static auto create(WideString contents) -> SourceFile;

    static auto from_file(const std::filesystem::path &path) -> Result<SourceFile, io::Error>;

    [[nodiscard]] auto get_name() const -> Option<Ref<String>>;

    [[nodiscard]] auto get_contents() const -> const WideString&;

    [[nodiscard]] auto slice(Range<> range) const -> WideString;

    [[nodiscard]] auto slice(usize from, usize to) const -> WideString;

    [[nodiscard]] auto clamp_index(usize i) const -> usize;

    [[nodiscard]] auto get_char(usize position) const -> widechar;

    [[nodiscard]] auto length() const -> usize;

    auto get_line(usize line_number) const -> Range<>;
  };
} // goos
