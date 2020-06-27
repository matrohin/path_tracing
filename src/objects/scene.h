#pragma once

#include "objects/material.h"
#include "objects/ray.h"
#include "objects/sphere.h"
#include "view/color.h"

#include <vector>

using Materials = std::vector<Material>;
using Objects = std::vector<Sphere>;

struct Scene {
  Color shoot_ray(Ray ray);

  Objects objects;
  Materials materials;
};
