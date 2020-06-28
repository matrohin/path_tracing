#pragma once

#include "geometry/point3d.h"
#include "geometry/vec3d.h"

#include <cstdint>

struct Ray;

struct Camera {
  Point3d start = {};
  Point3d left_bottom = {};
  double width_scale = 1;
  double height_scale = 1;
  Vec3d right = {1., 0., 0.};
  Vec3d up = {0., 1., 0.};

  Ray create_ray_from_pixel(uint32_t x, uint32_t y) const;
};
