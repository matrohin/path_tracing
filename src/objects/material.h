#pragma once

#include "view/color.h"

struct Material {
  Color diffuse = {};
  Color emmitance = {};
  double reflectivity = 0.;
  double transparency = 0.;
  double refraction_index = 0.;

  static Material create_light(const Color& c) { return {{}, c}; }
  static Material create_diffuse(const Color& c) { return {c, {}}; }
  static Material create_reflective(double r, const Color& c = {}) {
    return {c, {}, r};
  }
  static Material create_transparent(double t, double index) {
    return {{}, {}, 0., t, index};
  }
};
