#include "scene.h"

#include "geometry/utils.h"

#include <cmath>
#include <limits>
#include <optional>

// TODO: Refactor some parts into separate file?
namespace {
constexpr size_t not_found = std::numeric_limits<size_t>::max();
constexpr uint32_t max_depth = 10; // TODO: add command line argument

template <class TObjects>
std::pair<size_t, double> find_intersection(const TObjects& objects,
                                            const Ray& ray) {
  size_t best_idx = not_found;
  double best_distance = std::numeric_limits<double>::max();
  for (size_t i = 0; i < objects.size(); ++i) {
    const auto distance = objects[i].shape.intersect(ray);
    if (distance && *distance < best_distance) {
      best_idx = i;
      best_distance = *distance;
    }
  }
  return {best_idx, best_distance};
}

struct Hit {
  Point3d hit_point;
  size_t material_index = not_found;
  Vec3d normal;
};

Hit find_first_hit(const Scene& scene, const Ray& ray) {
  const auto best_sphere = find_intersection(scene.spheres, ray);
  const auto best_triangle = find_intersection(scene.triangles, ray);
  if (best_triangle.second < best_sphere.second) {
    const auto p = ray.at(best_triangle.second);
    const auto& t = scene.triangles[best_triangle.first];
    return Hit{p, t.material_index, t.shape.normal()};
  } else if (best_sphere.first != not_found) {
    const auto p = ray.at(best_sphere.second);
    const auto& s = scene.spheres[best_sphere.first];
    return Hit{p, s.material_index, s.shape.normalAtPoint(p)};
  }
  return {};
}

Color shoot_ray_impl(const Scene& scene, Ray ray, std::minstd_rand& rng,
                     uint32_t depth) {
  if (depth >= max_depth) return BLACK;

  const auto hit = find_first_hit(scene, ray);
  if (hit.material_index == not_found) return BLACK;

  const auto& mat = scene.materials[hit.material_index];
  const auto& normal = hit.normal;

  std::uniform_real_distribution<> distr;
  if (distr(rng) < mat.transparency) { // refract
    const auto new_dir = [&]() {
      const bool inside = ray.direction % normal < 0.;
      if (inside) {
        return refract_vec(ray.direction, -normal, mat.refraction_index, 1.);
      } else {
        return refract_vec(ray.direction, normal, 1., mat.refraction_index);
      }
    }();
    return mat.emmitance +
           shoot_ray_impl(scene, {hit.hit_point, new_dir}, rng, depth + 1);

  } else if (distr(rng) < mat.reflectivity) { // reflect
    const auto new_dir = reflect_vec(ray.direction, normal);
    return mat.emmitance +
           shoot_ray_impl(scene, {hit.hit_point, new_dir}, rng, depth + 1);

  } else { // diffuse
    const auto new_dir =
        generate_random_vec_on_hemisphere(normal, rng).normalized();
    const auto incoming =
        shoot_ray_impl(scene, {hit.hit_point, new_dir}, rng, depth + 1);
    const auto cos_theta = new_dir % normal;
    return mat.emmitance + mat.diffuse * cos_theta * incoming;
  }
}

} // unnamed namespace

Color shoot_ray(const Scene& scene, const Ray& ray, std::minstd_rand& engine) {
  return shoot_ray_impl(scene, ray, engine, 0);
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
  add_triangle({left_top, right_bottom, left_top + triangles.back().shape.first_edge}, material_index);
}
