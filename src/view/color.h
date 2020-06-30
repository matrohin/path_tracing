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

inline constexpr Color BLACK = {};
inline constexpr Color WHITE = {1., 1., 1.};
inline constexpr Color GREEN = {0., 1., 0.};
inline constexpr Color RED = {1., 0., 0.};
