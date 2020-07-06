#include "scene.h"

#include "geometry/utils.h"
#include "geometry/matrix3d.h"

#include <cmath>
#include <limits>
#include <optional>

// TODO: Refactor some parts into separate file?
namespace {
constexpr size_t not_found = std::numeric_limits<size_t>::max();

template <class TObjects>
std::pair<size_t, Intersection> find_intersection(const TObjects& objects,
                                                  const Ray& ray) {
  size_t best_idx = not_found;
  Intersection best_intersection = {std::numeric_limits<double>::max(), false};
  for (size_t i = 0; i < objects.size(); ++i) {
    const auto intersection = objects[i].shape.intersect(ray);
    if (intersection && intersection->distance < best_intersection.distance) {
      best_idx = i;
      best_intersection = *intersection;
    }
  }
  return {best_idx, best_intersection};
}

struct Hit {
  Point3d hit_point;
  size_t material_index = not_found;
  Vec3d normal;
  bool inside = false;
};

Hit find_first_hit(const Scene& scene, const Ray& ray) {
  const auto [sphere_idx, sphere_intersection] =
      find_intersection(scene.spheres, ray);
  const auto [triangle_idx, triangle_intersection] =
      find_intersection(scene.triangles, ray);

  if (triangle_intersection.distance < sphere_intersection.distance) {
    const auto p = ray.at(triangle_intersection.distance);
    const auto& t = scene.triangles[triangle_idx];
    const bool inside = triangle_intersection.inside;
    const auto n = inside ? -t.shape.normal() : t.shape.normal();
    return Hit{p, t.material_index, n, inside};

  } else if (sphere_idx != not_found) {
    const auto p = ray.at(sphere_intersection.distance);
    const auto& s = scene.spheres[sphere_idx];
    const bool inside = sphere_intersection.inside;
    const auto n = inside ? -s.shape.normalAt(p) : s.shape.normalAt(p);
    return Hit{p, s.material_index, n, inside};
  }
  return {};
}

} // unnamed namespace

Color shoot_ray(const Scene& scene, const Ray& ray, uint32_t depth,
                std::minstd_rand& rng) {
  if (depth == 0) return BLACK;

  const auto hit = find_first_hit(scene, ray);
  if (hit.material_index == not_found) return BLACK;

  const auto& mat = scene.materials[hit.material_index];
  const auto& normal = hit.normal;

  std::uniform_real_distribution<> distr;
  if (distr(rng) < mat.transparency) { // refract
    const auto new_dir = [&]() {
      if (hit.inside) {
        return refract_vec(ray.direction, normal, mat.refraction_index, 1.);
      } else {
        return refract_vec(ray.direction, normal, 1., mat.refraction_index);
      }
    }();
    return mat.emmitance +
           shoot_ray(scene, {hit.hit_point, new_dir}, depth - 1, rng);

  } else if (distr(rng) < mat.reflectivity) { // reflect
    const auto new_dir = reflect_vec(ray.direction, normal);
    return mat.emmitance +
           shoot_ray(scene, {hit.hit_point, new_dir}, depth - 1, rng);

  } else { // diffuse
    const auto new_dir = generate_random_vec_on_hemisphere(normal, 0., rng);
    const auto incoming =
        shoot_ray(scene, {hit.hit_point, new_dir}, depth - 1, rng);
    const auto cos_theta = new_dir % normal;
    return mat.emmitance + 2 * mat.diffuse * cos_theta * incoming;
  }
}

size_t Scene::add_material(const Material& m) {
  materials.push_back(m);
  return materials.size() - 1;
}

void Scene::add_sphere(const Sphere& s, size_t material_index) {
  spheres.push_back({s, material_index});
}

void Scene::add_triangle(const Triangle& t, size_t material_index) {
  triangles.push_back({t, material_index});
}

void Scene::add_rectangle(const Point3d& left_bottom, const Point3d& left_top,
                          const Point3d& right_bottom, size_t material_index) {
  add_triangle({left_bottom, right_bottom, left_top}, material_index);
  add_triangle(
      {left_top, right_bottom, left_top + triangles.back().shape.first_edge},
      material_index);
}
