#include "sphere.h"

#include "ray.h"

#include <cmath>
#include <iostream>

std::optional<Point3d> intersect(const Ray& ray, const Sphere& sphere) {
  const auto l = sphere.center - ray.start;
  const auto tc = l % ray.direction;
  if (tc < 0.) return {};

  const auto k2 = l % l - tc * tc;
  const auto r2 = sphere.radius * sphere.radius;
  if (k2 > r2) return {};

  const auto delta = sqrt(r2 - k2);
  const auto t0 = tc - delta;
  if (t0 > 0.) return ray.at(t0);

  const auto t1 = tc + delta;
  if (t1 > 0.) return ray.at(t1);

  return {};
}
