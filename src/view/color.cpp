#include "color.h"

Color operator*(const Color& a, double b) {
  return {a.r * b, a.g * b, a.b * b};
}

Color operator*(double a, const Color& b) { return b * a; }

Color operator*(const Color& a, const Color& b) {
  return {a.r * b.r, a.g * b.g, a.b * b.b};
}

Color operator+(const Color& a, const Color& b) {
  return {a.r + b.r, a.g + b.g, a.b + b.b};
}

