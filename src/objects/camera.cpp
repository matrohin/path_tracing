#include "camera.h"

#include "ray.h"

#include <cmath>

Camera::Camera(const Point3d& origin, const Vec3d& dir, const Vec3d& u,
               double field_of_view, double distance, uint32_t width,
               uint32_t height)
    : start(origin) {
  const auto aspect_ratio = 1. * width / height;

  const auto cam_h = 2. * std::tan(field_of_view) * distance;
  const auto cam_w = cam_h * aspect_ratio;
  up = u.normalized() * (cam_h / height);
  right = (u * dir).normalized() * (cam_w / width);

  const auto p = origin + dir.normalized() * distance;
  left_bottom = p - up * (height / 2.) - right * (width / 2.);
}

Ray Camera::create_ray_from_pixel(uint32_t x, uint32_t y) const {
  // TODO: add random
  const auto p = left_bottom + 1. * x * right + 1. * y * up;
  return {start, (p - start).normalized()};
}
