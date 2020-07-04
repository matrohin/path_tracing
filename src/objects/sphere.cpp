#include "sphere.h"

#include "ray.h"

#include <cmath>
#include <iostream>

Sphere::Sphere(const Point3d& c, double radius)
    : center(c), radius_sqr(radius * radius) {}

std::optional<double> Sphere::intersect(const Ray& ray) const {
  const auto l = center - ray.start;
  const auto tc = l % ray.direction;
  if (tc < 0.) return {};

  const auto k2 = l % l - tc * tc;
  if (k2 > radius_sqr) return {};

  const auto delta = sqrt(radius_sqr - k2);
  const auto t0 = tc - delta;
  if (t0 > 0.) return t0;

  const auto t1 = tc + delta;
  if (t1 > 0.) return t1;

  return {};
}
