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
  Color shoot_ray(const Ray& ray, std::mt19937& engine) const;

  Objects objects;
  Materials materials;
};
