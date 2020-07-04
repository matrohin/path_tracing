#pragma once

#include "geometry/point3d.h"

#include <optional>

struct Ray;
struct Intersection;

struct Sphere {
private:
  Point3d center = {};
  double radius_sqr = 0.0;

public:
  Sphere(const Point3d& c, double radius);
  std::optional<Intersection> intersect(const Ray& ray) const;
  Vec3d normalAt(const Point3d& point) const;
};

inline Vec3d Sphere::normalAt(const Point3d& point) const {
  return (point - center).normalized();
}
