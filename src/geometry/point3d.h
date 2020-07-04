#pragma once

#include "vec3d.h"

struct Point3d {
  double x = 0;
  double y = 0;
  double z = 0;
};

Point3d operator+(const Point3d& a, const Vec3d& b);
Point3d operator-(const Point3d& a, const Vec3d& b);
Vec3d operator-(const Point3d& a, const Point3d& b);

inline Point3d operator+(const Point3d& a, const Vec3d& b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z};
}
inline Point3d operator-(const Point3d& a, const Vec3d& b) {
  return a + (-b);
}
inline Vec3d operator-(const Point3d& a, const Point3d& b) {
  return {a.x - b.x, a.y - b.y, a.z - b.z};
}
