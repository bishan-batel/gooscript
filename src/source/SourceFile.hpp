//
// Created by bishan_ on 5/5/24.
//

#pragma once

#include <filesystem>
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

    static Result<SourceFile, io::Error> from_file(const std::filesystem::path &path);

    [[nodiscard]] Option<Ref<String>> get_name() const;

    [[nodiscard]] const String& get_contents() const;
  };
} // goos
