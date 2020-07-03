#pragma once

#include "objects/material.h"
#include "objects/ray.h"
#include "objects/sphere.h"
#include "objects/triangle.h"
#include "view/color.h"

#include <random>
#include <vector>

using Materials = std::vector<Material>;

struct Scene {
  template <class T> struct SceneObject {
    T shape;
    size_t material_index = 0;
  };

  std::vector<SceneObject<Sphere>> spheres;
  std::vector<SceneObject<Triangle>> triangles;
  std::vector<Material> materials;

  size_t add_material(const Material& m);
  void add_sphere(const Sphere& s, size_t material_index);
  void add_triangle(const Triangle& t, size_t material_index);
  void add_rectangle(const Point3d& left_bottom, const Point3d& left_top,
                     const Point3d& right_bottom, size_t material_index);
};

Color shoot_ray(const Scene& scene, const Ray& ray, uint32_t depth, std::minstd_rand& engine);

