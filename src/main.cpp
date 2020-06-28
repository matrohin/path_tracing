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
  Objects objects{Sphere{Point3d{0., -50., 270.}, 25.},
                  Sphere{Point3d{100., 100., 240.}, 25.},
                  Sphere{Point3d{-20., -50., 220.}, 25.},
                  Sphere{Point3d{-100., 100., 230.}, 25.},
                  Sphere{Point3d{0., 50., 200.}, 5.},
                  Sphere{Point3d{0., 0., 325.}, 20.},
                  Sphere{Point3d{-50., 175., 225.}, 65.}};
  Materials materials{Material{GREEN},       Material{RED},   Material{GREEN},
                      Material{RED},         Material{GREEN}, Material{RED},
                      Material{WHITE, WHITE}};
  return {std::move(objects), std::move(materials)};
}

constexpr uint32_t samples_num = 10000;
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

View generate_image() {
  Scene scene = build_scene();
  Camera camera{Point3d{}, Point3d{-50., -50., 100}, 0.2, 0.2};

  View view{500, 500};
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

  std::string file_name;
  app.add_option("-f", file_name, "Output png file");

  CLI11_PARSE(app, argc, argv);

  const auto image = generate_image();
  png_utils::write_png(file_name.c_str(), image);

  return 0;
}
