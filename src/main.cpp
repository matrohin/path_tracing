#include "geometry/point3d.h"
#include "geometry/vec3d.h"
#include "images/png_utils.h"
#include "objects/camera.h"
#include "objects/ray.h"
#include "objects/render.h"
#include "objects/scene.h"
#include "objects/sphere.h"
#include "view/view.h"

#include <CLI/CLI.hpp>
#include <chrono>
#include <iostream>
#include <string>

namespace {

Scene build_default_scene() {
  Scene scene;
  const auto light_gray_diffuse =
      scene.add_material(Material::create_diffuse({0.85, 0.85, 0.85}));
  const auto red_diffuse =
      scene.add_material(Material::create_diffuse({0.75, 0.25, 0.25}));
  const auto blue_diffuse =
      scene.add_material(Material::create_diffuse({0.25, 0.25, 0.75}));
  const auto green_diffuse =
      scene.add_material(Material::create_diffuse({0.25, 0.75, 0.25}));
  const auto blue_green_diffuse =
      scene.add_material(Material::create_diffuse({0., 0.90, 0.75}));
  const auto perfect_reflection =
      scene.add_material(Material::create_reflective(1.));
  const auto yellowish_reflection =
      scene.add_material(Material::create_reflective(0.5, {0.8, 0.8, 0.}));
  const auto glass = scene.add_material(Material::create_transparent(1.0, 1.5));
  const auto light =
      scene.add_material(Material::create_light({7.0, 6.0, 6.0}));

  scene.add_rectangle({35., 100., 65.}, {65., 100., 65.}, {35., 100., 35.},
                      light);
  scene.add_rectangle({0., 0., -100.}, {0., 0., 100.}, {0., 100., -100.},
                      red_diffuse); // left
  scene.add_rectangle({100., 0., 100.}, {100., 0., -100.}, {100., 100., 100.},
                      blue_diffuse); // right

  scene.add_sphere({{50., 50., 1e5 + 100.}, 1e5}, light_gray_diffuse);  // front
  scene.add_sphere({{50., 50., -1e5 - 101.}, 1e5}, light_gray_diffuse); // back
  scene.add_sphere({{50., -1e5, 0.}, 1e5}, light_gray_diffuse);       // bottom
  scene.add_sphere({{50., 1e6 + 100., 0.}, 1e6}, light_gray_diffuse); // top

  scene.add_sphere({{23., 11., 40.}, 11.}, blue_green_diffuse);   // sphere 1
  scene.add_sphere({{75., 23., 78.}, 23.}, perfect_reflection);   // sphere 2
  scene.add_sphere({{23., 30., 40.}, 8.}, green_diffuse);         // sphere 3
  scene.add_sphere({{49., 14., 55.}, 14.}, yellowish_reflection); // sphere 4
  scene.add_sphere({{33., 22., 20.}, 13.}, glass);                // sphere 5

  return scene;
}

Camera create_default_camera(uint32_t width, uint32_t height) {
  return {Point3d{50., 50., -100.},
          Vec3d{0., 0., 1.},
          Vec3d{0., 1., 0.},
          0.5135,
          1.,
          width,
          height};
}

} // unnamed namespace

int main(int argc, char** argv) {
  CLI::App app{"Path Tracing Toy Project"};

  uint32_t width;
  uint32_t height;
  uint32_t samples_num;
  uint32_t depth;
  std::string file_name;
  app.set_help_flag("--help", "");
  app.add_option("-f,--file", file_name, "Output png file")->required();
  app.add_option("-w,--width", width, "Output file width")->default_val(640);
  app.add_option("-h,--height", height, "Output file height")->default_val(480);
  app.add_option("-s,--samples", samples_num, "Number of samples per pixel")
      ->default_val(100);
  app.add_option("-d,--depth", depth,
                 "Maximum recursive depth of transmitting/reflecting rays")
      ->default_val(10);

  CLI11_PARSE(app, argc, argv);

  const auto start = std::chrono::steady_clock::now();

  const RenderingContext context{create_default_camera(width, height),
                                 build_default_scene(), samples_num, depth};
  const auto end_scene = std::chrono::steady_clock::now();

  View image{width, height};
  render(image, context);
  const auto end_image = std::chrono::steady_clock::now();

  png_utils::write_png(file_name.c_str(), image);
  const auto end_full = std::chrono::steady_clock::now();

  const auto get_diff = [](const auto& s, const auto& e) {
    return std::chrono::duration<double>(e - s).count();
  };
  std::cout << "Scene build: " << get_diff(start, end_scene) << "s.\n";
  std::cout << "Image generation: " << get_diff(end_scene, end_image) << "s.\n";
  std::cout << "Png image creation: " << get_diff(end_image, end_full)
            << "s.\n";
  std::cout << "Full program execution: " << get_diff(start, end_full)
            << "s.\n";
  return 0;
}
