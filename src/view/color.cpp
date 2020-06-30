#include "color.h"

Color& Color::operator/=(double o) {
  r /= o;
  g /= o;
  b /= o;
  return *this;
}

Color& Color::operator+=(const Color& o) {
  r += o.r;
  g += o.g;
  b += o.b;
  return *this;
}

Color operator*(const Color& a, double b) {
  return {a.r * b, a.g * b, a.b * b};
}

Color operator*(double a, const Color& b) {
  return b * a;
}

Color operator*(const Color& a, const Color& b) {
  return {a.r * b.r, a.g * b.g, a.b * b.b};
}

Color operator+(const Color& a, const Color& b) {
  Color res = a;
  res += b;
  return res;
}
