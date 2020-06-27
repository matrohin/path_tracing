#pragma once

#include "vec3d.h"

struct Point3d {
  double x = 0;
  double y = 0;
  double z = 0;
};

Point3d operator+(const Point3d& a, const Vec3d& b);
Vec3d operator-(const Point3d& a, const Point3d& b);
