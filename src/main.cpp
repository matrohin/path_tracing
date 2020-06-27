#include "geometry/point3d.h"
#include "geometry/vec3d.h"
#include "objects/ray.h"
#include "objects/sphere.h"
#include "view/view.h"
#include "images/png_utils.h"

#include <CLI/CLI.hpp>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
  CLI::App app{"Path Tracing Toy Project"};

  std::string file_name;
  app.add_option("-f", file_name, "Output png file");

  CLI11_PARSE(app, argc, argv);

  View image{ 100, 100 };
  for (uint32_t x = 0; x < image.width; ++x) {
    image(x, 2) = image(x, 3) = image(x, 4) = Color{ 0, 0xff, 0 };
  }

  png_utils::write_png(file_name.c_str(), image);


  const Ray ray{Point3d{0., 0., 0.}, Vec3d{1., 1., 0.}.normalized()};
  const Sphere s{Point3d{4.5, 3., 0.}, 1.};
  const auto i = intersect(ray, s);
  if (i) {
    std::cout << "Yes: " << i->x << ' ' << i->y << ' ' << i->z << '\n';
  } else {
    std::cout << "No\n";
  }
  return 0;
}
