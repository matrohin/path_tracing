#include "vec3d.h"

#include <cmath>

double Vec3d::length() const {
  return sqrt(*this%*this);
}
Vec3d Vec3d::normalized() const {
  return *this / length();
}
double operator%(const Vec3d& a, const Vec3d& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}
Vec3d operator*(const Vec3d& a, double b) {
  return {a.x * b, a.y * b, a.z * b};
}
Vec3d operator/(const Vec3d& a, double b) {
  return a * (1 / b);
}
