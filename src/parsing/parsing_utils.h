#pragma once

#include "parsing/error.h"

#include <algorithm>
#include <memory>
#include <string>
#include <string_view>

struct Color;
struct Vec3d;
struct Point3d;

namespace parsing {

inline std::string operator+(std::string a, std::string_view b) {
  a.append(b);
  return a;
}

[[nodiscard]] std::string_view trim_spaces(std::string_view s);
[[nodiscard]] std::string_view trim_comment(std::string_view s);
[[nodiscard]] bool remove_prefix(std::string_view& s, std::string_view prefix);

[[nodiscard]] int parse_int(std::string_view& s);
[[nodiscard]] double parse_double(std::string_view& s);
[[nodiscard]] Color parse_color(std::string_view& s);
[[nodiscard]] Point3d parse_point(std::string_view& s);
[[nodiscard]] Vec3d parse_vec(std::string_view& s);

template <class T> void for_all_lines(const char* file_name, T func) {
  using namespace std::string_literals;

  std::unique_ptr<FILE, decltype(fclose)*> file{fopen(file_name, "r"), fclose};
  if (!file) throw std::runtime_error{"Cannot open file "s + file_name};

  std::string cur_line;
  size_t line_idx = 1;
  const auto process_buf = [&](const char* buf_start, const char* buf_end) {
    auto buf_p = buf_start;
    while (true) {
      const auto it = std::find(buf_p, buf_end, '\n');
      cur_line.append(buf_p, it);

      if (it == buf_end) return;

      func(line_idx++, trim_spaces(trim_comment(cur_line)));
      cur_line.clear();
      buf_p = std::next(it);
    }
  };

  char buf[256];
  while (true) {
    const auto count =
        std::fread(buf, sizeof(char), std::size(buf), file.get());
    process_buf(buf, buf + count);
    if (count < std::size(buf)) break;
  }
  func(line_idx, trim_spaces(trim_comment(cur_line)));
}

template <class TFunc>
void for_all_lines_with_error(const char* file_name, TFunc func) {
  for_all_lines(file_name, [&](size_t line_idx, std::string_view line) {
    try {
      func(line);
    } catch (const std::exception& err) {
      throw ParsingError{file_name, err.what(), line_idx};
    }
  });
}

} // namespace parsing
