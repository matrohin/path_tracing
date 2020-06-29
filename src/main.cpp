#include "geometry/point3d.h"
#include "geometry/vec3d.h"
#include "images/png_utils.h"
#include "objects/camera.h"
#include "objects/ray.h"
#include "objects/scene.h"
#include "objects/sphere.h"
#include "view/view.h"

#include <CLI/CLI.hpp>
#include <chrono>
#include <future>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

Scene build_scene() {
  auto scene = Scene::with_capacity(10);
  scene.add_sphere({{-1e5, 50., 0.}, 1e5},
                   Material::create_diffuse({0.75, 0.25, 0.25}));  // left
  scene.add_sphere({{1e5 + 100.0, 50., 0.}, 1e5},
                   Material::create_diffuse({0.25, 0.25, 0.75}));  // right
  scene.add_sphere({{50., 50., 1e5 + 100.}, 1e5},
                   Material::create_diffuse({0.75, 0.75, 0.75}));  // front
  scene.add_sphere({{50., 50., -1e5 - 101.}, 1e5},
                   Material::create_diffuse({0.75, 0.75, 0.75}));  // back
  scene.add_sphere({{50., -1e5, 0.}, 1e5},
                   Material::create_diffuse({0.75, 0.75, 0.75}));  // bottom
  scene.add_sphere({{50., 1e6 + 100., 0.}, 1e6},
                   Material::create_diffuse({0.75, 0.75, 0.75}));  // top

  scene.add_sphere({{27., 16., 47.}, 16.},
                   Material::create_diffuse({0., 0.90, 0.75}));  // sphere 1
  scene.add_sphere({{73., 23., 78.}, 23.},
                   Material::create_diffuse({0.9, 0.9, 0.9}));  // sphere 2
  scene.add_sphere({{73., 56., 78.}, 10.},
                   Material::create_diffuse({0.9, 0.9, 0.9}));  // sphere 3
  scene.add_sphere({{50., 399., 50.}, 300.},
                   Material::create_light({4.0, 3.0, 3.0}));  // light

  return scene;
}

constexpr uint32_t samples_num = 100;
ViewRow generate_row(const Scene& scene, const Camera& camera, uint32_t y,
                     uint32_t width, std::mt19937& rng_engine) {
  ViewRow row(width);
  for (uint32_t x = 0; x < width; ++x) {
    for (uint32_t i = 0; i < samples_num; ++i) {
      row[x] += scene.shoot_ray(camera.create_ray_from_pixel(x, y), rng_engine);
    }
    row[x] /= (1.0 * samples_num);
  }

  return row;
}

void generate_multiple_rows(const Scene& scene, const Camera& camera,
                            View& view, uint32_t first, uint32_t last,
                            uint32_t seed) {
  std::mt19937 rng_engine{seed};
  for (auto y = first; y < last; ++y) {
    view.apply_row(generate_row(scene, camera, y, view.width, rng_engine),
                    y);
  }
}

View generate_image(const Scene& scene, const Camera& camera, uint32_t w,
                    uint32_t h) {
  View view{w, h};

  const auto max_threads = std::thread::hardware_concurrency();
  std::cout << "Number of threads: " << max_threads << '\n';

  const uint32_t batch_size = view.height / max_threads;
  std::vector<std::thread> threads;
  threads.reserve(max_threads - 1);
  uint32_t seed = 0;
  for (uint32_t t = 0; t < max_threads - 1; ++t) {
    const uint32_t first = t * batch_size;
    const uint32_t last = first + batch_size;
    threads.emplace_back(generate_multiple_rows, std::cref(scene),
                         std::cref(camera), std::ref(view), first, last,
                         seed++);
  }

  generate_multiple_rows(scene, camera, view, (max_threads - 1) * batch_size,
                         view.height, seed++);

  for (auto& t : threads) t.join();

  return view;
}

int main(int argc, char** argv) {
  CLI::App app{"Path Tracing Toy Project"};

  uint32_t width;
  uint32_t height;
  std::string file_name;
  app.set_help_flag("--help", "");
  app.add_option("-f,--file", file_name, "Output png file")->required();
  app.add_option("-w,--width", width, "Output file width")->default_val(640);
  app.add_option("-h,--height", height, "Output file height")->default_val(480);

  CLI11_PARSE(app, argc, argv);

  const auto start = std::chrono::steady_clock::now();
  const Camera camera{Point3d{50., 50., -100.},
                      Vec3d{0., 0., 1.},
                      Vec3d{0., 1., 0.},
                      0.5135,
                      1.,
                      width,
                      height};
  const Scene scene = build_scene();
  const auto end_scene = std::chrono::steady_clock::now();

  const auto image = generate_image(scene, camera, width, height);
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
