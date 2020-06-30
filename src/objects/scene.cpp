#include "scene.h"

#include "geometry/utils.h"

#include <cmath>
#include <limits>
#include <optional>

namespace {
constexpr size_t not_found = std::numeric_limits<size_t>::max();
constexpr uint32_t max_depth = 10;

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

Color shoot_ray_impl(const Scene& scene, Ray ray, std::mt19937& engine,
                     uint32_t depth) {
  if (depth >= max_depth) return BLACK;

  const auto [idx, distance] = find_intersection(scene.objects, ray);
  if (idx == not_found) return BLACK;

  const auto p = ray.at(distance);
  const auto normal = (p - scene.objects[idx].center).normalized();
  const auto new_dir =
      generate_random_vec_on_hemisphere(normal, engine).normalized();

  const auto& mat = scene.materials[idx];
  const auto incoming = shoot_ray_impl(scene, {p, new_dir}, engine, depth + 1);
  const auto cos_theta = new_dir % normal;
  return mat.emmitance + mat.diffuse * cos_theta * incoming;
}

} // unnamed namespace

Scene Scene::with_capacity(size_t cap) {
  Scene res;
  res.objects.reserve(cap);
  res.materials.reserve(cap);
  return res;
}

Color Scene::shoot_ray(const Ray& ray, std::mt19937& engine) const {
  return shoot_ray_impl(*this, ray, engine, 0);
}

void Scene::add_sphere(const Sphere& s, const Material& m) {
  objects.push_back(s);
  materials.push_back(m);
}
