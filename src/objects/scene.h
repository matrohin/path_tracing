#pragma once

#include "objects/material.h"
#include "objects/ray.h"
#include "objects/sphere.h"
#include "view/color.h"

#include <vector>
#include <random>

using Materials = std::vector<Material>;
using Objects = std::vector<Sphere>;

struct Scene {
  Objects objects;
  Materials materials;

  static Scene with_capacity(size_t cap);

  Color shoot_ray(const Ray& ray, std::mt19937& engine) const;
  void add_sphere(const Sphere& s, const Material& m);
};
