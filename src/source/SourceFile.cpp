//
// Created by bishan_ on 5/5/24.
//

#include "SourceFile.hpp"

#include <fstream>
#include <ref.hpp>
#include <sstream>

namespace goos {
  Result<SourceFile, io::Error> SourceFile::from_file(const std::filesystem::path &path) {
    errno = 0;
    const std::ifstream stream{path};

    if (stream.bad()) return err(io::Error{errno});;

    return crab::ok(
      SourceFile(
        path.string(),
        (WideStringStream{} << stream.rdbuf()).str()
      )
    );
  }

  Option<Ref<String>> SourceFile::get_name() const {
    if (name->is_none()) return crab::none;
    return crab::some(Ref{name->get_unchecked()});
  }

  const WideString& SourceFile::get_contents() const { return contents; }

  SourceFile::SourceFile(String name, WideString contents)
    : name{crab::some(std::move(name))}, contents{std::move(contents)} {}

  SourceFile::SourceFile(WideString contents)
    : name{crab::none}, contents{std::move(contents)} {}

  SourceFile SourceFile::create(String name, WideString contents) {
    return SourceFile{std::move(name), std::move(contents)};
  }
} // goos
