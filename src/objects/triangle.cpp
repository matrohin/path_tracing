#include "triangle.h"

#include "ray.h"

#include <cmath>

Triangle::Triangle(const Point3d& a, const Point3d& b, const Point3d& c)
    : vertex(a), first_edge(b - a), second_edge(c - a) {}

std::optional<double> Triangle::intersect(const Ray& ray) const {
  const auto pvec = ray.direction * second_edge;
  const auto det = first_edge % pvec;

  if (abs(det) < 1e-8) return {};

  const auto inv_det = 1. / det;
  const auto tvec = ray.start - vertex;
  const auto u = tvec % pvec * inv_det;
  if (u < 0. || u > 1.) return {};

  const auto qvec = tvec * first_edge;
  const auto v = ray.direction % qvec * inv_det;
  if (v < 0. || u + v > 1.) return {};

  const auto t = second_edge % qvec * inv_det;
  if (t > 0) return t;
  return {};
}

Vec3d Triangle::normal() const {
  return (first_edge * second_edge).normalized();
}

