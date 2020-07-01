#pragma once

#include "geometry/point3d.h"

#include <optional>

struct Ray;

struct Sphere {
private:
  Point3d center = {};
  double radius_sqr = 0.0;

public:
  Sphere(const Point3d& c, double radius);
  std::optional<double> intersect(const Ray& ray) const;
  Vec3d normalAtPoint(const Point3d& point) const;
};

