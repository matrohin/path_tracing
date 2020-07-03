#include "render.h"

#include "objects/scene.h"

#include <random>
#include <thread>

namespace {

ViewRow render_row(const RenderingContext& context, uint32_t y, uint32_t width,
                   std::minstd_rand& rng) {
  const auto& camera = context.camera;
  const auto& scene = context.scene;
  const auto samples_num = context.samples_num;
  const auto depth = context.max_depth;

  ViewRow row(width);
  for (uint32_t x = 0; x < width; ++x) {
    Color color = BLACK;
    for (uint32_t i = 0; i < samples_num; ++i) {
      color +=
          shoot_ray(scene, camera.create_ray_from_pixel(x, y, rng), depth, rng);
    }
    row[x] = color / (1.0 * samples_num);
  }

  return row;
}

void render_multiple_rows(View& view, const RenderingContext& context,
                          uint32_t first, uint32_t last, uint32_t seed) {
  std::minstd_rand rng_engine{seed};
  for (auto y = first; y < last; ++y) {
    view.apply_row(render_row(context, y, view.width, rng_engine), y);
  }
}

} // unnamed namespace

void render(View& view, const RenderingContext& context) {
  const auto max_threads =
      std::thread::hardware_concurrency(); // TODO: Make it command line
                                           // argument
  const uint32_t batch_size = view.height / max_threads;
  std::vector<std::thread> threads;
  threads.reserve(max_threads - 1);
  uint32_t seed = 0;
  for (uint32_t t = 0; t < max_threads - 1; ++t) {
    const uint32_t first = t * batch_size;
    const uint32_t last = first + batch_size;
    threads.emplace_back(render_multiple_rows, std::ref(view),
                         std::cref(context), first, last, seed++);
  }

  render_multiple_rows(view, context, (max_threads - 1) * batch_size,
                       view.height, seed++);

  for (auto& t : threads) t.join();
}

