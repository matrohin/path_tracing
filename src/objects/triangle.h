#pragma once

#include "geometry/point3d.h"

#include <optional>

struct Ray;

struct Triangle {
  Point3d vertex;
  Vec3d first_edge;
  Vec3d second_edge;

  Triangle(const Point3d& a, const Point3d& b, const Point3d& c);
  std::optional<double> intersect(const Ray& ray) const;
  Vec3d normal() const;
};

