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

  WideStringView SourceFile::slice(const Range<> range) const {
    return slice(range.lower_bound(), range.upper_bound());
  }

  WideStringView SourceFile::slice(const usize from, const usize to) const {
    return WideStringView{*contents}.substr(clamp_index(from), clamp_index(to - from));
  }

  usize SourceFile::clamp_index(const usize i) const {
    return std::min(i, contents->size());
  }

  widechar SourceFile::get_char(const usize position) const {
    return contents->at(clamp_index(position));
  }

  usize SourceFile::length() const {
    return contents->size();
  }

  SourceFile::SourceFile(String name, WideString contents)
    : name{crab::some(std::move(name))}, contents{std::move(contents)} {}

  SourceFile::SourceFile(WideString contents)
    : name{crab::none}, contents{std::move(contents)} {}

  SourceFile SourceFile::create(String name, WideString contents) {
    return SourceFile{std::move(name), std::move(contents)};
  }

  SourceFile SourceFile::create(WideString contents) {
    return SourceFile{std::move(contents)};
  }
} // goos
