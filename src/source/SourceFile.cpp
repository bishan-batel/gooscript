//
// Created by bishan_ on 5/5/24.
//

#include "SourceFile.hpp"

#include <fstream>
#include <ref.hpp>
#include <sstream>
#include <sstream>
#include <fstream>
#include <codecvt>

namespace goos {
  auto SourceFile::from_file(const std::filesystem::path &path) -> Result<SourceFile, io::Error> {
    errno = 0;
    std::wifstream stream{path};

    if (stream.bad()) return err(io::Error{errno});;

    stream.imbue(std::locale(std::locale{}, new std::codecvt_utf8<wchar_t>));
    WideStringStream collector{};
    collector << stream.rdbuf();

    return crab::ok(
      SourceFile(
        path.string(),
        collector.str()
      )
    );
  }

  auto SourceFile::get_name() const -> Option<Ref<String>> {
    if (name->is_none()) return crab::none;
    return crab::some(Ref{name->get_unchecked()});
  }

  auto SourceFile::get_contents() const -> const WideString& { return contents; }

  auto SourceFile::slice(const Range<> range) const -> WideString {
    return slice(range.lower_bound(), range.upper_bound());
  }

  auto SourceFile::slice(const usize from, const usize to) const -> WideString {
    return contents->substr(clamp_index(from), clamp_index(to - from));
  }

  auto SourceFile::clamp_index(const usize i) const -> usize {
    return std::min(i, contents->size() - 1);
  }

  auto SourceFile::get_char(const usize position) const -> widechar {
    return contents->at(clamp_index(position));
  }

  auto SourceFile::length() const -> usize {
    return contents->size();
  }

  auto SourceFile::get_line(const usize line_number) const -> Range<> {
    usize index{0}, line{1};
    for (const widechar c: *contents) {
      if (line == line_number) break;

      if (c == '\n') {
        line++;
      }
      index++;
    }

    usize end;
    for (end = index; get_char(end) != '\n'; end++) {}
    return crab::range(index, end);
  }

  SourceFile::SourceFile(String name, WideString contents)
    : name{crab::make_rc<Option<String>>(crab::some(std::move(name)))},
      contents{crab::make_rc<WideString>(std::move(contents))} {}

  SourceFile::SourceFile(WideString contents)
    : name{crab::make_rc<Option<String>>(crab::none)},
      contents{crab::make_rc<WideString>(std::move(contents))} {}

  auto SourceFile::create(String name, WideString contents) -> SourceFile {
    return SourceFile{std::move(name), std::move(contents)};
  }

  auto SourceFile::create(WideString contents) -> SourceFile {
    return SourceFile{std::move(contents)};
  }
} // goos
