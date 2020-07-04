#pragma once

#include <cmath>

struct Vec3d {
  double x = 0;
  double y = 0;
  double z = 0;

  double length() const;
  Vec3d normalized() const;
};

double operator%(const Vec3d& a, const Vec3d& b);
Vec3d operator*(const Vec3d& a, const Vec3d& b);
Vec3d operator*(const Vec3d& a, double b);
Vec3d operator*(double a, const Vec3d& b);
Vec3d operator/(const Vec3d& a, double b);
Vec3d operator-(const Vec3d& a);
Vec3d operator+(const Vec3d& a, const Vec3d& b);
Vec3d operator-(const Vec3d& a, const Vec3d& b);

inline double Vec3d::length() const {
  return sqrt(*this % *this);
}
inline Vec3d Vec3d::normalized() const {
  return *this / length();
}
inline double operator%(const Vec3d& a, const Vec3d& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}
inline Vec3d operator*(const Vec3d& a, const Vec3d& b) {
  return {a.y * b.z - b.y * a.z, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}
inline Vec3d operator*(const Vec3d& a, double b) {
  return {a.x * b, a.y * b, a.z * b};
}
inline Vec3d operator*(double a, const Vec3d& b) {
  return b * a;
}
inline Vec3d operator/(const Vec3d& a, double b) {
  return a * (1 / b);
}
inline Vec3d operator-(const Vec3d& a) {
  return {-a.x, -a.y, -a.z};
}
inline Vec3d operator+(const Vec3d& a, const Vec3d& b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z};
}
inline Vec3d operator-(const Vec3d& a, const Vec3d& b) {
  return a + (-b);
}
