#pragma once

#include "geometry/point3d.h"
#include "geometry/vec3d.h"

#include <cstdint>

struct Ray;

struct Camera {
  Point3d start = {};
  Point3d left_bottom = {};
  Vec3d right = {1., 0., 0.};
  Vec3d up = {0., 1., 0.};

  Camera(const Point3d& origin, const Vec3d& dir, const Vec3d& u,
         double field_of_view, double distance, uint32_t width,
         uint32_t height);

  Ray create_ray_from_pixel(uint32_t x, uint32_t y) const;
};
