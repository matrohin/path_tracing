#include "camera.h"

#include "ray.h"

Ray Camera::create_ray_from_pixel(uint32_t x, uint32_t y) {
  // TODO: add random
  const auto p = left_bottom + x * width_scale * right + y * height_scale * up;
  return {p, (p - start).normalized()};
}
