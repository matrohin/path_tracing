#include "view.h"

View::View(uint32_t w, uint32_t h) : width(w), height(h), pixels(h) {}

Color& View::operator()(uint32_t x, uint32_t y) { return pixels[y][x]; }
const Color& View::operator()(uint32_t x, uint32_t y) const {
  return pixels[y][x];
}

void View::apply_row(ViewRow&& i_row, uint32_t y) {
  pixels[y] = std::move(i_row);
}

