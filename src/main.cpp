#include "geometry/point3d.h"
#include "geometry/vec3d.h"
#include "images/png_utils.h"
#include "objects/camera.h"
#include "objects/ray.h"
#include "objects/scene.h"
#include "objects/sphere.h"
#include "view/view.h"

#include <CLI/CLI.hpp>
#include <iostream>
#include <string>
#include <vector>

Scene build_scene() {
  Objects objects{Sphere{Point3d{0., -50., 250.}, 25.},
                  Sphere{Point3d{0., 0., 325.}, 50.}};
  Materials materials{Material{GREEN}, Material{RED}};
  return {std::move(objects), std::move(materials)};
}

View generate_image() {
  Scene scene = build_scene();
  Camera camera{Point3d{}, Point3d{-50., -50., 100}, 0.2, 0.2};

  View view{500, 500};
  for (uint32_t y = 0; y < view.height; ++y) {
    for (uint32_t x = 0; x < view.width; ++x) {
      view(x, y) = scene.shoot_ray(camera.create_ray_from_pixel(x, y));
    }
  }
  return view;
}

int main(int argc, char** argv) {
  CLI::App app{"Path Tracing Toy Project"};

  std::string file_name;
  app.add_option("-f", file_name, "Output png file");

  CLI11_PARSE(app, argc, argv);

  const auto image = generate_image();
  png_utils::write_png(file_name.c_str(), image);

  return 0;
}
