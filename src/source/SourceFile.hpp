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
    static SourceFile create(String name, WideString contents);

    static SourceFile create(WideString contents);

    static Result<SourceFile, io::Error> from_file(const std::filesystem::path &path);

    [[nodiscard]] Option<Ref<String>> get_name() const;

    [[nodiscard]] const WideString& get_contents() const;

    [[nodiscard]] WideStringView slice(Range<> range) const;

    [[nodiscard]] WideStringView slice(usize from, usize to) const;

    [[nodiscard]] usize clamp_index(usize i) const;

    [[nodiscard]] widechar get_char(usize position) const;

    [[nodiscard]] usize length() const;
  };
} // goos
