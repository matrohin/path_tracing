#include "scene.h"

#include <limits>
#include <optional>

namespace {
constexpr size_t not_found = std::numeric_limits<size_t>::max();

std::pair<size_t, double> find_intersection(const Objects& objects,
                                             const Ray& ray) {
  size_t best_idx = not_found;
  double best_distance = std::numeric_limits<double>::max();
  for (size_t i = 0; i < objects.size(); ++i) {
    const auto distance = intersect(ray, objects[i]);
    if (distance && *distance < best_distance) {
      best_idx = i;
      best_distance = *distance;
    }
  }
  return {best_idx, best_distance};
}
}  // unnamed namespace

Color Scene::shoot_ray(Ray ray) {
  const auto idx = find_intersection(objects, ray).first;
  if (idx == not_found) return BLACK;
  return materials[idx].color;
}
