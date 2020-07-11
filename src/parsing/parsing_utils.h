#pragma once

#include "geometry/point3d.h"
#include "geometry/vec3d.h"
#include "parsing/error.h"
#include "view/color.h"

#include <algorithm>
#include <charconv>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <string_view>

// TODO: Move some functions and using to cpp/another file
using namespace std::string_literals;

namespace parsing {

inline std::string operator+(std::string a, std::string_view b) {
  a.append(b);
  return a;
}

inline void remove_front_spaces(std::string_view& s) {
  s.remove_prefix(std::min(s.size(), s.find_first_not_of(" \n\r\t")));
}

inline std::string_view trim_spaces(std::string_view s) {
  remove_front_spaces(s);
  const auto last = s.find_last_not_of(" \n\r\t");
  if (last != std::string::npos) s.remove_suffix(s.size() - last - 1);
  return s;
}

inline std::string_view trim_comment(std::string_view s) {
  const auto idx = s.find('#');
  if (idx != std::string::npos) s.remove_suffix(s.size() - idx);
  return s;
}
template <class T> void for_all_lines(const char* file_name, T func) {
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

template <size_t size>
[[nodiscard]] bool remove_prefix(std::string_view& s,
                                 const char (&prefix)[size]) {
  if (s.substr(0, size - 1) == prefix) {
    s.remove_prefix(size - 1);
    return true;
  }
  return false;
}

inline int parse_int(std::string_view& s) {
  remove_front_spaces(s);

  int v = 0;
  const auto r = std::from_chars(s.data(), s.data() + s.size(), v);
  if (r.ec != std::errc{}) {
    throw std::runtime_error{"Couldn't parse integer value in "s + s};
  }
  s.remove_prefix(r.ptr - s.data());
  if (!s.empty()) s.remove_prefix(1);
  return v;
}

inline double parse_double(std::string_view& s) {
  remove_front_spaces(s);

  // This should be a simple call to from_chars but neither gcc nor clang
  // support this in current trunk, so, do UNSAFE strtod instead. It's ok,
  // because we know that in s there are symbols after double values.
  char* end = nullptr;
  const auto v = std::strtod(s.data(), &end);
  if (errno != 0)
    throw std::runtime_error{"Couldn't parse double value in "s + s};
  s.remove_prefix(end - s.data());
  return v;
}

inline Color parse_color(std::string_view& s) {
  Color res;
  res.r = parse_double(s);
  res.g = parse_double(s);
  res.b = parse_double(s);
  return res;
}

inline Point3d parse_point(std::string_view& s) {
  Point3d res;
  res.x = parse_double(s);
  res.y = parse_double(s);
  res.z = parse_double(s);
  return res;
}

inline Vec3d parse_vec(std::string_view& s) {
  Vec3d res;
  res.x = parse_double(s);
  res.y = parse_double(s);
  res.z = parse_double(s);
  return res;
}

} // namespace parsing
