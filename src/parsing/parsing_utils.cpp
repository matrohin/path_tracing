#include "parsing_utils.h"

#include "geometry/point3d.h"
#include "geometry/vec3d.h"
#include "view/color.h"

#include <charconv>

using namespace std::string_literals;

namespace parsing {

namespace {

const auto whitespace_chars = " \n\r\t";

void remove_front_spaces(std::string_view& s) {
  s.remove_prefix(std::min(s.size(), s.find_first_not_of(whitespace_chars)));
}

void remove_trailing_spaces(std::string_view& s) {
  const auto idx = s.find_last_not_of(whitespace_chars);
  if (idx != std::string::npos) s.remove_suffix(s.size() - idx - 1);
}

template <class T> T parse_3d_object(std::string_view& s) {
  const auto x = parse_double(s);
  const auto y = parse_double(s);
  const auto z = parse_double(s);
  return {x, y, z};
}

} // unnamed namespace

std::string_view trim_spaces(std::string_view s) {
  remove_front_spaces(s);
  remove_trailing_spaces(s);
  return s;
}

std::string_view trim_comment(std::string_view s) {
  const auto idx = s.find('#');
  if (idx != std::string::npos) s.remove_suffix(s.size() - idx);
  return s;
}

bool remove_prefix(std::string_view& s, std::string_view prefix) {
  if (s.substr(0, prefix.size()) == prefix) {
    s.remove_prefix(prefix.size());
    return true;
  }
  return false;
}

int parse_int(std::string_view& s) {
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

double parse_double(std::string_view& s) {
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

Color parse_color(std::string_view& s) {
  return parse_3d_object<Color>(s);
}

Point3d parse_point(std::string_view& s) {
  return parse_3d_object<Point3d>(s);
}

Vec3d parse_vec(std::string_view& s) {
  return parse_3d_object<Vec3d>(s);
}

} // namespace parsing
