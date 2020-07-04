#pragma once

struct Color {
  double r = 0.;
  double g = 0.;
  double b = 0.;
  Color& operator/=(double o);
  Color& operator+=(const Color& o);
};

Color operator*(const Color& a, double b);
Color operator*(double a, const Color& b);
Color operator*(const Color& a, const Color& b);
Color operator+(const Color& a, const Color& b);
Color operator/(Color a, double b);

inline constexpr Color BLACK = {};
inline constexpr Color WHITE = {1., 1., 1.};
inline constexpr Color GREEN = {0., 1., 0.};
inline constexpr Color RED = {1., 0., 0.};

inline Color& Color::operator/=(double o) {
  r /= o;
  g /= o;
  b /= o;
  return *this;
}

inline Color& Color::operator+=(const Color& o) {
  r += o.r;
  g += o.g;
  b += o.b;
  return *this;
}

inline Color operator*(const Color& a, double b) {
  return {a.r * b, a.g * b, a.b * b};
}

inline Color operator*(double a, const Color& b) {
  return b * a;
}

inline Color operator*(const Color& a, const Color& b) {
  return {a.r * b.r, a.g * b.g, a.b * b.b};
}

inline Color operator+(const Color& a, const Color& b) {
  Color res = a;
  res += b;
  return res;
}

inline Color operator/(Color a, double b) {
  return a /= b;
}
