#include "geometry/point3d.h"
#include "geometry/vec3d.h"
#include "images/png_utils.h"
#include "parsing/camloader.h"
#include "parsing/error.h"
#include "parsing/objloader.h"
#include "rendering/camera.h"
#include "rendering/render.h"
#include "rendering/scene.h"
#include "view/view.h"

#include <CLI/CLI.hpp>
#include <chrono>
#include <cstdio>
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
  const auto blue_green_diffuse =
      scene.add_material(Material::create_diffuse({0., 0.90, 0.75}));
  const auto perfect_reflection =
      scene.add_material(Material::create_reflective(1.));
  const auto yellowish_reflection =
      scene.add_material(Material::create_reflective(0.5, {0.8, 0.8, 0.}));
  const auto glass = scene.add_material(Material::create_transparent(1.0, 1.5));
  const auto light =
      scene.add_material(Material::create_light({7.0, 6.0, 6.0}));

  scene.add_rectangle({35., 99.99, -65.}, {65., 99.99, -65.},
                      {35., 99.99, -35.}, light);

  // left and right sides are not in clockwise order to test that inside of
  // object intersections are working too
  scene.add_rectangle({0., 0., 100.}, {0., 0., -100.}, {0., 100., 100.},
                      red_diffuse); // left
  scene.add_rectangle({100., 0., -100.}, {100., 0., 100.}, {100., 100., -100.},
                      blue_diffuse); // right
  scene.add_rectangle({0., 0., -100.}, {0., 100., -100.}, {100., 0., -100.},
                      light_gray_diffuse); // front
  scene.add_rectangle({100., 0., 100.}, {100., 100., 100.}, {0., 0., 100.},
                      light_gray_diffuse); // back
  scene.add_rectangle({0., 0., 100.}, {0., 0., -100.}, {100., 0., 100.},
                      light_gray_diffuse); // bottom
  scene.add_rectangle({0., 100., -100.}, {0., 100., 100.}, {100., 100., -100.},
                      light_gray_diffuse); // top

  scene.add_sphere({{23., 20., -40.}, 20.}, blue_green_diffuse);
  scene.add_sphere({{23., 50., -40.}, 8.}, glass);
  scene.add_sphere({{75., 23., -78.}, 23.}, perfect_reflection);
  scene.add_sphere({{49., 14., -55.}, 14.}, yellowish_reflection);

  return scene;
}

Camera create_default_camera(uint32_t width, uint32_t height) {
  return {Point3d{50., 50., 100.},
          Vec3d{0., 0., -1.},
          Vec3d{0., 1., 0.},
          0.5135,
          1.,
          width,
          height};
}

RenderingContext make_rendering_context(const std::string& cam_file,
                                        const std::string& obj_file,
                                        uint32_t samples_num, uint32_t depth,
                                        uint32_t width, uint32_t height) {
  try {
    return RenderingContext{
        cam_file.empty()
            ? create_default_camera(width, height)
            : parsing::load_cam_file(cam_file.c_str(), width, height),
        obj_file.empty() ? build_default_scene()
                         : parsing::load_obj_file(obj_file.c_str()),
        samples_num, depth};
  } catch (const parsing::ParsingError& err) {
    printf("Error while parsing %s(%zu): %s\n", err.file_name.c_str(), err.line,
           err.reason.c_str());
    throw;
  }
}

} // unnamed namespace

int main(int argc, char** argv) {
  CLI::App app{"Path Tracing Toy Project"};

  uint32_t width;
  uint32_t height;
  uint32_t samples_num;
  uint32_t depth;
  std::string png_file;
  std::string obj_file;
  std::string cam_file;
  app.set_help_flag("--help", "");
  app.add_option("-f,--file", png_file, "Output .png file")->required();
  app.add_option("-i,--input", obj_file, "Input .obj file");
  app.add_option("-c,--camera", cam_file, "Input .cam file");
  app.add_option("-w,--width", width, "Output file width")->default_val(640);
  app.add_option("-h,--height", height, "Output file height")->default_val(480);
  app.add_option("-s,--samples", samples_num, "Number of samples per pixel")
      ->default_val(100);
  app.add_option("-d,--depth", depth,
                 "Maximum recursive depth of transmitting/reflecting rays")
      ->default_val(10);

  CLI11_PARSE(app, argc, argv);

  const auto start = std::chrono::steady_clock::now();

  const RenderingContext context = make_rendering_context(
      cam_file, obj_file, samples_num, depth, width, height);

  const auto end_scene = std::chrono::steady_clock::now();

  View image{width, height};
  render(image, context);
  const auto end_image = std::chrono::steady_clock::now();

  png_utils::write_png(png_file.c_str(), image);
  const auto end_full = std::chrono::steady_clock::now();

  const auto get_diff = [](const auto& s, const auto& e) {
    return std::chrono::duration<double>(e - s).count();
  };

  printf("Scene build: %lfs.\n", get_diff(start, end_scene));
  printf("Image generation: %lfs.\n", get_diff(end_scene, end_image));
  printf("Png image creation: %lfs.\n", get_diff(end_image, end_full));
  printf("Full program execution: %lfs.\n", get_diff(start, end_full));
  return 0;
}
