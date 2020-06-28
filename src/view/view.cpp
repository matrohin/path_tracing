#include "view.h"

View::View(uint32_t w, uint32_t h)
    : width(w), height(h), pixels(w * h) {}

Color& View::operator()(uint32_t x, uint32_t y) { return pixels[y * width + x]; }
const Color& View::operator()(uint32_t x, uint32_t y) const {
  return pixels[y * width + x];
}

void View::apply_row(const ViewRow& i_row, uint32_t y) {
  // TODO_performance: Should we store vector of vectors instead and avoid copying?
  std::copy(i_row.begin(), i_row.end(), pixels.begin() + y * width);
}

