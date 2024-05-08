//
// Created by bishan_ on 5/5/24.
//

#include "SourceFile.hpp"

#include <fstream>
#include <ref.hpp>
#include <sstream>

namespace goos {
  auto SourceFile::from_file(const std::filesystem::path &path) -> Result<SourceFile, io::Error> {
    errno = 0;
    const std::wifstream stream{path};

    if (stream.bad()) return err(io::Error{errno});;

    return crab::ok(
      SourceFile(
        path.string(),
        (WideStringStream{} << stream.rdbuf()).str()
      )
    );
  }

  auto SourceFile::get_name() const -> Option<Ref<String>> {
    if (name->is_none()) return crab::none;
    return crab::some(Ref{name->get_unchecked()});
  }

  auto SourceFile::get_contents() const -> const WideString& { return contents; }

  auto SourceFile::slice(const Range<> range) const -> WideStringView {
    return slice(range.lower_bound(), range.upper_bound());
  }

  auto SourceFile::slice(const usize from, const usize to) const -> WideStringView {
    return WideStringView{*contents}.substr(clamp_index(from), clamp_index(to - from));
  }

  auto SourceFile::clamp_index(const usize i) const -> usize {
    return std::min(i, contents->size());
  }

  auto SourceFile::get_char(const usize position) const -> widechar {
    return contents->at(clamp_index(position));
  }

  auto SourceFile::length() const -> usize {
    return contents->size();
  }

  SourceFile::SourceFile(String name, WideString contents)
    : name{crab::some(std::move(name))}, contents{std::move(contents)} {}

  SourceFile::SourceFile(WideString contents)
    : name{crab::none}, contents{std::move(contents)} {}

  auto SourceFile::create(String name, WideString contents) -> SourceFile {
    return SourceFile{std::move(name), std::move(contents)};
  }

  auto SourceFile::create(WideString contents) -> SourceFile {
    return SourceFile{std::move(contents)};
  }
} // goos
