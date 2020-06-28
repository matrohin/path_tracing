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
                     uint32_t width, uint32_t seed) {
  std::mt19937 rng_engine{seed};
  ViewRow row(width);
  for (uint32_t x = 0; x < width; ++x) {
    for (uint32_t i = 0; i < samples_num; ++i) {
      row[x] += scene.shoot_ray(camera.create_ray_from_pixel(x, y), rng_engine);
    }
    row[x] /= (1.0 * samples_num);
  }

  return row;
}

template <typename T>
bool is_ready(const std::future<T>& f) {
  return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

View generate_image(const Camera& camera, const Scene& scene, uint32_t w,
                    uint32_t h) {
  View view{w, h};

  std::vector<std::pair<uint32_t, std::future<ViewRow>>> futures;
  const auto max_threads = std::thread::hardware_concurrency();
  std::cout << "Number of threads: " << max_threads << '\n';

  uint32_t y = 0;
  uint32_t seed = 0;
  const auto max_y = view.height;
  while (y < max_y) {
    while (futures.size() < max_threads) {
      futures.emplace_back(y, std::async(std::launch::async, generate_row,
                                         scene, camera, y, view.width, seed));
      ++y;
      ++seed;
    }
    for (auto it = futures.begin(); it != futures.end();) {
      if (is_ready(it->second)) {
        view.apply_row(it->second.get(), it->first);
        it = futures.erase(it);
      } else {
        ++it;
      }
    }
  }
  for (auto& [idx, fut] : futures) {
    view.apply_row(fut.get(), idx);
  }
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

  const Camera camera{Point3d{50., 50., -100.},
                      Vec3d{0., 0., 1.},
                      Vec3d{0., 1., 0.},
                      0.5135,
                      1.,
                      width,
                      height};
  const Scene scene = build_scene();

  const auto start = std::chrono::steady_clock::now();

  const auto image = generate_image(camera, scene, width, height);

  const auto diff =
      std::chrono::duration<double>{std::chrono::steady_clock::now() - start};
  std::cout << "Image generated in " << diff.count() << "s.\n";

  png_utils::write_png(file_name.c_str(), image);

  return 0;
}
