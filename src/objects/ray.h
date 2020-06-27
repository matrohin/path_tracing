#pragma once

#include "geometry/point3d.h"
#include "geometry/vec3d.h"

struct Ray {
  Point3d start = {};
  Vec3d direction = {};

  Point3d at(double t) const;
};
