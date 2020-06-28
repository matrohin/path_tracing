#pragma once

#include "view/color.h"

struct Material {
  Color diffuse = {};
  Color emmitance = {};

  static Material create_light(const Color& c) { return {{}, c}; }
  static Material create_diffuse(const Color& c) { return {c, {}}; }
};
