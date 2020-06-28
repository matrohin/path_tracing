#include "point3d.h"

Point3d operator+(const Point3d& a, const Vec3d& b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z};
}
Point3d operator-(const Point3d& a, const Vec3d& b) {
  return a + (-b);
}
Vec3d operator-(const Point3d& a, const Point3d& b) {
  return {a.x - b.x, a.y - b.y, a.z - b.z};
}
