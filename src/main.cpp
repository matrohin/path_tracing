#include "geometry/point3d.h"
#include "geometry/vec3d.h"
#include "objects/ray.h"
#include "objects/sphere.h"

#include <CLI/CLI.hpp>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
  CLI::App app{"Path Tracing Toy Project"};

  std::string name;
  app.add_option("-n", name, "Name");

  CLI11_PARSE(app, argc, argv);

  std::cout << "Hello, " << name << '\n';

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
