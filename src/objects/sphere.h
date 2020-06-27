#pragma once

#include "geometry/point3d.h"

#include <optional>

struct Ray;

struct Sphere {
  Point3d center = {};
  double radius = 0.0;
};

std::optional<double> intersect(const Ray& ray, const Sphere& sphere);
